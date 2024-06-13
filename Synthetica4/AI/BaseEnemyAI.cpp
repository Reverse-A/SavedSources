// ©2023 Reverse-A. All rights reserved.

#include "BaseEnemyAI.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "Synthetica/Components/AttributeComponent.h"
#include "Synthetica/HUD/HealthBarComponent.h"
#include "Synthetica/AI/BaseAIAnimInstance.h"
#include "Synthetica/Weapons/MeleeWeapons/BaseMeleeWeapon.h"
#include "Synthetica/Weapons/Weapon.h"
#include "MotionWarpingComponent.h"

ABaseEnemyAI::ABaseEnemyAI()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensing->SetPeripheralVisionAngle(30.f);
	PawnSensing->SightRadius = 4000.f;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	AI_MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

}


void ABaseEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	AIState = EAIState::EAIS_Initialize;
	AI_MaxWalkSpeed = 100.f;

	Termination = false;
	bIsDead = false;
	bIsCheckNextTargetInIdle = false;
	bHit = false;
	bHitTermination = false;
	bCombatStarted = false;
	bIsChasing = false;
	bIsPatrolling = false;

	if (HealthBarWidget) HealthBarWidget->SetVisibility(false);

	EnemyAIController = Cast<AAIController>(GetController());
	
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ABaseEnemyAI::PawnSeen);
	}
}

void ABaseEnemyAI::Tick(float DeltaTime)
{

	if (Termination) return;
	Super::Tick(DeltaTime);
	Call_DeltaTime = DeltaTime;
	AI_GeneralCalculations();

	//UE_LOG(LogTemp, Warning, TEXT("AI_MaxWalkSpeed: %f"), AI_MaxWalkSpeed);
	//UE_LOG(LogTemp, Warning, TEXT("AIState: %s"), *UEnum::GetValueAsString<EAIState>(AIState));
}


void ABaseEnemyAI::AttachWeapon()
{
	UWorld* World = GetWorld();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (World && MeleeWeaponClass && MeleeAIClass)
	{
		ABaseMeleeWeapon* DefaultWeapon = World->SpawnActor<ABaseMeleeWeapon>(MeleeWeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("MeleeWeaponSocketRight"), this, this);
		MeleeEquippedWeapon = DefaultWeapon;
	}

	if (World && RangeWeaponClass && RangeAIClass)
	{
		AWeapon* DefaultRangeWeapon = World->SpawnActor<AWeapon>(RangeWeaponClass);
		DefaultRangeWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		RangeEquippedWeapon = DefaultRangeWeapon;
	}

	if (MeleeDisarmedWeapon)
	{
		MeleeDisarmedWeapon->SetActorHiddenInGame(true);
	}

	if (RangeDisarmedWeapon)
	{
		RangeDisarmedWeapon->SetActorHiddenInGame(true);
	}

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(WeaponArmDisarmMontage);
		AnimInstance->Montage_JumpToSection(FName("Arm"));
	}

	bWeaponAttached = true;

}

void ABaseEnemyAI::DetachWeapon()
{
	UWorld* World = GetWorld();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (World && MeleeWeaponClass && MeleeAIClass)
	{
		ABaseMeleeWeapon* DefaultWeapon = World->SpawnActor<ABaseMeleeWeapon>(MeleeWeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("MeleeWeaponSheatSocketRight"), this, this);
		MeleeDisarmedWeapon = DefaultWeapon;
	}
	if (World && RangeWeaponClass && RangeAIClass)
	{
		AWeapon* DefaultRangeWeapon = World->SpawnActor<AWeapon>(RangeWeaponClass);
		DefaultRangeWeapon->Equip(GetMesh(), FName("RangeWeaponSheatSocketRight"), this, this);
		RangeDisarmedWeapon = DefaultRangeWeapon;
	}
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(WeaponArmDisarmMontage);
		AnimInstance->Montage_JumpToSection(FName("Disarm"));
	}
	if (MeleeEquippedWeapon)
	{
		MeleeEquippedWeapon->SetActorHiddenInGame(true);
	}

	if (RangeEquippedWeapon)
	{
		RangeEquippedWeapon->SetActorHiddenInGame(true);
	}

	bWeaponAttached = false;
}

void ABaseEnemyAI::AI_GeneralCalculations()
{
	Attributes&& Attributes->IsAlive() ? bIsDead = false : bIsDead = true;

	bIsDead ? AI_DeathConditions() : AI_AliveConditions();
	GetCharacterMovement()->MaxWalkSpeed = AI_MaxWalkSpeed;


	if (bHit && !bHitTermination)
	{
		if(!bCombatStarted) bCombatStarted = true;
		HitSituation();
		bHitTermination = true;
	}
	if (bCombatStarted && !bInCombatDistance && !bCombatTarget_IsDetected)
	{
		CombatDuration();
	}
}

void ABaseEnemyAI::AI_AliveConditions()
{
	if (bIsDead) AIState = EAIState::EAIS_Death;
	IntervalResults();
	switch (AIState)
	{
	case EAIState::EAIS_Initialize:
		AI_InitialState();
		break;

	case EAIState::EAIS_Idle:
		AI_IdleState();
		break;

	case EAIState::EAIS_Patrolling:
		AI_PatrollingState();
		break;

	case EAIState::EAIS_ChaseState:
		AI_ChaseState();
		break;

	case EAIState::EAIS_AlertState:
		AI_AlertState();
		break;

	case EAIState::EAIS_Injured:
		break;

	case EAIState::EAIS_Death:
		AI_DeathConditions();
		break;
	}
}


void ABaseEnemyAI::AI_DeathConditions()
{
	EnemyAIController->StopMovement();
	HealthBarWidget->SetVisibility(false);
	GetCapsuleComponent()->SetActive(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	this->GetMesh()->SetSimulatePhysics(true);
	this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	HealthBarWidget = nullptr;
	CombatTarget = nullptr;
	PatrolTarget = nullptr;

	Termination = true;
}

void ABaseEnemyAI::IntervalResults()
{

	if (InTargetRange(CombatTarget, MeleeAttackRadius)) bInMeleeAttackDistance = true;
	else if (!InTargetRange(CombatTarget, MeleeAttackRadius)) bInMeleeAttackDistance = false;

	if (InTargetRange(CombatTarget, RangeAttackRadius)) bInRangeAttackDistance = true;
	else if (!InTargetRange(CombatTarget, RangeAttackRadius)) bInRangeAttackDistance = false;

	if (InTargetRange(CombatTarget, CombatRadius)) bInCombatDistance = true;
	else if (!InTargetRange(CombatTarget, CombatRadius)) bInCombatDistance = false;

	if (InTargetRange(PatrolTarget, PatrolRadius)) bInPatrolDistance = true;
	else if (InTargetRange(PatrolTarget, PatrolRadius)) bInPatrolDistance = false;

}

void ABaseEnemyAI::AI_InitialState()
{
	AI_MoveToTarget(PatrolTarget);
	bool IsStarted = true;
	if (bInPatrolDistance && IsStarted)
	{
		IsStarted = false;
		AIState = EAIState::EAIS_Idle;
	}
}

void ABaseEnemyAI::AI_IdleState()
{
	if (!bIsCheckNextTargetInIdle)
	{	
		if (HealthBarWidget) HealthBarWidget->SetVisibility(false);
		bCombatTarget_IsDetected = false;
		bIsPatrolling = false;
		bIsChasing	  = false;
		CombatTarget = nullptr;
		EnemyAIController->StopMovement();
		AI_ControlNextPatrolTarget();
		bIsCheckNextTargetInIdle = true;
		FTimerHandle IdleToPatrolTimer;
		const float PatrolTimeSwitchDelay = FMath::RandRange(3.f, 5.f);
		GetWorldTimerManager().SetTimer(IdleToPatrolTimer, this, &ABaseEnemyAI::AI_IdleToPatrol, PatrolTimeSwitchDelay);
		GetWorldTimerManager().ClearTimer(AlertStateOverTimerHandle);
	}
}

void ABaseEnemyAI::AI_IdleToPatrol()
{
	AIState = EAIState::EAIS_Patrolling;
}


void ABaseEnemyAI::AI_PatrollingState()
{
	if (!bIsPatrolling && bIsCheckNextTargetInIdle)
	{
		if (HealthBarWidget) HealthBarWidget->SetVisibility(false);
		AI_MaxWalkSpeed = 100.f;
		AI_MoveToTarget(PatrolTarget);
		bIsPatrolling = true;
		bIsChasing = false;
		bCombatTarget_IsDetected = false;
	}
	NextPatrolTargetDelay += Call_DeltaTime;
	const float NextPatrolTargetDelayLimit = 15.f;
	if (bIsPatrolling && bInPatrolDistance && NextPatrolTargetDelay >= NextPatrolTargetDelayLimit)
	{
		EnemyAIController->StopMovement();
		bIsPatrolling = false;
		bIsCheckNextTargetInIdle = false;
		const float ToIdleSwitchTime = FMath::RandRange(2.f, 5.f);
		GetWorldTimerManager().SetTimer(PatrolToIdleTimerHandle, this, &ABaseEnemyAI::AI_PatrolToIdle, ToIdleSwitchTime);
		NextPatrolTargetDelay = 0.f;
	}
}

void ABaseEnemyAI::AI_PatrolToIdle()
{
	AIState = EAIState::EAIS_Idle;
}

void ABaseEnemyAI::AI_ChaseState()
{
	if (AIState == EAIState::EAIS_AlertState) return;
	GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
	GetWorldTimerManager().ClearTimer(PatrolToIdleTimerHandle);

	if (CombatTarget)
	{
		if (RangeAIClass)
		{
			AI_MaxWalkSpeed = 200.f;
		}
		else if (MeleeAIClass)
		{
			AI_MaxWalkSpeed = 450.f;
		}

		if (bInCombatDistance && !bIsChasing)
		{
			if (HealthBarWidget) HealthBarWidget->SetVisibility(true);
			AI_MoveToTarget(CombatTarget);
			bIsChasing = true;
			if (PatrolTarget != nullptr) PatrolTarget = nullptr;
		}
		if(!bHitTermination) AI_MoveToTarget(CombatTarget);

		if (!bInCombatDistance && bIsChasing)
		{
			TransitionToAlertStateTimer += Call_DeltaTime;

			if (TransitionToAlertStateTimer >= TransitionToAlertStateDelay)
			{
				AIState = EAIState::EAIS_AlertState;
			}
		}
	}
}


void ABaseEnemyAI::AI_AlertState()
{
	if (bIsChasing)
	{
		AI_MaxWalkSpeed = 50.0f;
		CombatTarget = nullptr;

		if (HealthBarWidget) HealthBarWidget->SetVisibility(true);
		EnemyAIController->StopMovement();
		bCombatTarget_IsDetected = false;
		bIsChasing = false;

		const float AlertTimeDelay = FMath::RandRange(3.f, 5.f);
		GetWorldTimerManager().SetTimer(AlertStateOverTimerHandle, this, &ABaseEnemyAI::AI_AlertStateOver, AlertTimeDelay);
	}
}

void ABaseEnemyAI::AI_AlertStateOver()
{
	bIsCheckNextTargetInIdle = false;
	AIState = EAIState::EAIS_Idle;
}

void ABaseEnemyAI::PawnSeen(APawn* SeenPawn)
{
	if (AIState == EAIState::EAIS_ChaseState) return;
	if (!bCombatTarget_IsDetected)
	{
		if (SeenPawn->ActorHasTag(FName("BaseCharacter")))
		{
			GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
			GetWorldTimerManager().ClearTimer(PatrolToIdleTimerHandle);
			CombatTarget = SeenPawn;
			bCombatTarget_IsDetected = true;
			if (bHit) return;
			else 
			AIState = EAIState::EAIS_ChaseState;
		}
	}
}
void ABaseEnemyAI::NoiseSense(APawn* NoiseTarget, const FVector& Location, float Volume)
{

}

void ABaseEnemyAI::HitSituation()
{

	if (bIsDead) return;
	AIState = EAIState::EAIS_ChaseState;
	EnemyAIController->StopMovement();
	GetWorldTimerManager().SetTimer(HitResetTimerHandle, this, &ABaseEnemyAI::ResetHit, 1.f, false);
}

void ABaseEnemyAI::ResetHit()
{
	AI_MoveToTarget(CombatTarget);
	bHitTermination = false;
	bHit = false;
	
}

void ABaseEnemyAI::CombatDuration()
{
	if(AIState == EAIState::EAIS_ChaseState || bIsDead) return;
	GetWorldTimerManager().SetTimer(CombatTimerHandle, this, &ABaseEnemyAI::CombatEnd, FMath::RandRange(10.f, 20.f), false);
}

void ABaseEnemyAI::CombatEnd()
{
	bCombatStarted = false;
	GetWorldTimerManager().ClearTimer(CombatTimerHandle);
}


// AI_Combat Functions
void ABaseEnemyAI::GetHit(const FVector& ImpactPoint)
{
	if (bIsDead) return;
	if (!bHit) bHit = true;
	DirectionalHitReaction(ImpactPoint);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

float ABaseEnemyAI::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

void ABaseEnemyAI::PlayHitReactMontage(const FName& SectionName)
{
	if (bIsDead) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseEnemyAI::DirectionalHitReaction(const FVector& ImpactPoint)
{
	if (bIsDead) return;
	const FVector Forward = GetActorForwardVector();
	//Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward* ToHit = |Forward| |ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from raidans to degree
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct poinst down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);


	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	if (Theta >= -45.f && Theta < 45.f)
	{
		//Front
		SetHitReactValue = 0.f;
		SetDeathValue = 0.f;
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		//Left
		SetHitReactValue = 1.f;
		SetDeathValue = 1.f;
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		//Right
		SetHitReactValue = 2.f;
		SetDeathValue = 2.f;
	}
	else if (Theta >= 135.f && Theta > -135.f)
	{
		//Back
		SetHitReactValue = 3.f;
		SetDeathValue = 3.f;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Section: %s"), *Section.ToString());
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	//}
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	//if (GetWorld())
	//{
	//	DrawDebugSphere(GetWorld(), ImpactPoint, 5.f, 12, FColor::Red, true);
	//}
}

// PatrolTarget || CombatTarget Functions

bool ABaseEnemyAI::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;

	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

AActor* ABaseEnemyAI::AI_NextPatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

void ABaseEnemyAI::AI_ControlNextPatrolTarget()
{
	PatrolTarget = AI_NextPatrolTarget();
}

void ABaseEnemyAI::AI_MoveToTarget(AActor* Target)
{
	if (EnemyAIController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnemyAIController->MoveTo(MoveRequest);
}

void ABaseEnemyAI::AI_StopMovement()
{
	EnemyAIController->StopMovement();
}

void ABaseEnemyAI::AI_Melee_AttackEnd()
{
	bCanMeleeAttack = true;
	if (!bInMeleeAttackDistance)
	{
		AIState = EAIState::EAIS_ChaseState;
	}
	else
	{
		AIState = EAIState::EAIS_AttackState;
	}
}
void ABaseEnemyAI::SetWarpTarget()
{
	if (CombatTarget)
	{
		FTransform CombatTargetTransform = CombatTarget->GetActorTransform();
		FName CombatTargetName = "CombatTarget";
		MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(CombatTargetName, CombatTargetTransform);
	}
}