// @2023 All rights reversed by Reverse-Alpha Studios


#include "GameModes/CaptureTheFlag/Objects/Flag.h"

#include "Characters/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerController/PlayerControllerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "Net/UnrealNetwork.h"
#include "SyntheticA/SyntheticA.h"

AFlag::AFlag()
{
	PrimaryActorTick.bCanEverTick   = false;
	bReplicates						= true;
	bNetUseOwnerRelevancy			= true;
	bAlwaysRelevant					= false;
	SetReplicateMovement(true);

	FlagStaticMesh	= CreateDefaultSubobject<UStaticMeshComponent>		(TEXT("FlagMesh"));
	FlagAreaSphere	= CreateDefaultSubobject<USphereComponent>			(TEXT("OverlapSphere"));
	ScoreSphere		= CreateDefaultSubobject<USphereComponent>			(TEXT("ScoreAreaSphere"));

	SetRootComponent									(FlagStaticMesh);
	FlagAreaSphere->SetupAttachment						(FlagStaticMesh);

	FlagStaticMesh->SetCollisionResponseToAllChannels	(ECollisionResponse::ECR_Block);
	FlagStaticMesh->SetCollisionResponseToChannel		(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	FlagStaticMesh->SetCollisionEnabled					(ECollisionEnabled::NoCollision);

	FlagAreaSphere->SetCollisionResponseToAllChannels	(ECollisionResponse::ECR_Ignore);
	FlagAreaSphere->SetCollisionEnabled					(ECollisionEnabled::NoCollision);
	FlagAreaSphere->SetSphereRadius						(250.0f);

	ScoreSphere->SetCollisionEnabled					(ECollisionEnabled::NoCollision);
	ScoreSphere->SetCollisionResponseToAllChannels		(ECollisionResponse::ECR_Ignore);
	ScoreSphere->SetupAttachment						(FlagAreaSphere);

	ScoreSphere->SetWorldLocation						(FVector::ZeroVector);
}

void AFlag::BeginPlay()
{
	Super::BeginPlay();

	FlagAreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FlagAreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	FlagAreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AFlag::OnSphereOverlap);
	FlagAreaSphere->OnComponentEndOverlap.AddDynamic(this, &AFlag::OnSphereEndOverlap);

	FlagFirstLocation = GetActorLocation();
	FlagFirstRotation = GetActorRotation();
}

void AFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FlagControlPoints(DeltaTime);
}

void AFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFlag, FlagState);
	DOREPLIFETIME_CONDITION(AFlag, TeamFlagColour, COND_OwnerOnly);
}

void AFlag::FlagControlPoints(float DeltaSeconds)
{
	CheckFlagStateTime += DeltaSeconds;
	float Delay = 1.5f;


	bool CheckFlagState = false;
	if (CheckFlagState)
	{
		if (CheckFlagStateTime > Delay)
		{
			FString FlagStateString = UEnum::GetValueAsString<EFlagState>(FlagState);

			UE_LOG(LogTemp, Warning, TEXT("CTFFlagState is: %s"), *FlagStateString);
			CheckFlagStateTime = 0.0f;
		}

		if (!bFlagIsDropped)
		{
			if (DestroyFlag_TimerHandle.IsValid())
			{
				GetWorldTimerManager().ClearTimer(DestroyFlag_TimerHandle);
				UE_LOG(LogTemp, Warning, TEXT("Timer Cleared"));
			}
		}
	}
	else
	{
		if (CheckFlagStateTime > Delay)
		{
			FString FlagColourString = UEnum::GetValueAsString<ETeamFlagColour>(TeamFlagColour);
			UE_LOG(LogTemp, Warning, TEXT("TeamFlagColour is: %s"), *FlagColourString);
			CheckFlagStateTime = 0.0f;
		}
	}
}

void AFlag::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (bFlagOverlappedWithScorePoint == false)
	{
		BaseCharacter = Cast<ABaseCharacter>(OtherActor);
		if (BaseCharacter)
		{
			if (BaseCharacter->IsHoldingTheFlag()) return;

			BaseCharacter->Set_OverlappingFlag(this);
			APlayerControllerManager* PlayerControllerManager = Cast<APlayerControllerManager>(BaseCharacter->Controller);
			if (PlayerControllerManager)
			{
				PlayerControllerManager->SetHUDFlag(true);
			}
		}
		bFlagOverlappedWithScorePoint = true;
	}
}

void AFlag::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{

	if (bFlagOverlappedWithScorePoint == true)
	{
		BaseCharacter = Cast<ABaseCharacter>(OtherActor);
		if (BaseCharacter)
		{
			if (BaseCharacter->IsHoldingTheFlag()) return;

			BaseCharacter->Set_OverlappingFlag(nullptr);
			APlayerControllerManager* PlayerControllerManager = Cast<APlayerControllerManager>(BaseCharacter->Controller);
			if (PlayerControllerManager)
			{
				PlayerControllerManager->SetHUDFlag(false);
			}
		}
		bFlagOverlappedWithScorePoint = false;
	}
	
}


void AFlag::OnState_Flag()
{
	switch (FlagState)
	{
	case EFlagState::EFS_Initial:
		OnState_Flag_Initial();
		break;
	case EFlagState::EFS_Equiped:
		OnState_Flag_Equipped();
		break;
	case EFlagState::EFS_Dropped:
		OnState_Flag_Dropped();
		break;
	case EFlagState::EFS_Scored:
		OnState_Flag_Scored();
		break;
	}
}

void AFlag::OnRep_FlagState()
{
	OnState_Flag();
}

void AFlag::OnState_Flag_Initial()
{
	FlagAreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FlagAreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AFlag::OnState_Flag_Equipped()
{
	PlaySound_Equip_Flag();

	FlagAreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlagStaticMesh->SetSimulatePhysics(false);
	FlagStaticMesh->SetEnableGravity(false);
	FlagStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlagStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	ScoreSphere->SetCollisionObjectType(ECC_Flag);
	ScoreSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ScoreSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ScoreSphere->SetCollisionResponseToChannel(ECC_Flag, ECollisionResponse::ECR_Overlap);
	ScoreSphere->SetSphereRadius(100.0f);
	ScoreSphere->ShapeColor.Green;

	if (bFlagOverlappedWithScorePoint == true)
	{
		if (BaseCharacter)
		{
			BaseCharacter->Set_OverlappingFlag(nullptr);

			APlayerControllerManager* PlayerControllerManager = Cast<APlayerControllerManager>(BaseCharacter->Controller);
			if (PlayerControllerManager)
			{
				PlayerControllerManager->SetHUDFlag(false);
			}
		}
		bFlagOverlappedWithScorePoint = false;
	}

	if (bFlagIsDropped)
	{
		bFlagIsDropped = false;
	}
}

void AFlag::OnState_Flag_Dropped()
{
	if (HasAuthority())
	{
		FlagAreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	FlagStaticMesh->SetSimulatePhysics(true);
	FlagStaticMesh->SetEnableGravity(true);
	FlagStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FlagStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	FlagStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	bFlagIsDropped = true;

	if (bFlagIsDropped)
	{
		GetWorldTimerManager().SetTimer
		(
			DestroyFlag_TimerHandle,
			this,
			&AFlag::TimerHandler_DestroyFlag,
			FlagDestroyAfterDroppedTime
		);
	}
}


void AFlag::OnState_Flag_Scored()
{
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	FlagStaticMesh->DetachFromComponent(DetachRules);

	ABaseCharacter* FlagHolder = Cast<ABaseCharacter>(GetOwner());
	if (FlagHolder)
	{
		FlagHolder->Set_HoldingFlag(false);
	}
	PlaySound_ScoreFlag();
	SetOwner(nullptr);
	Set_FlagState(EFlagState::EFS_Initial);
}

void AFlag::TimerHandler_DestroyFlag()
{
	Destroy();
}

void AFlag::Set_FlagState(EFlagState State)
{
	FlagState = State;
	OnState_Flag();
}

void AFlag::Drop_Flag()
{
	Set_FlagState(EFlagState::EFS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	FlagStaticMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
}
#pragma region PlaySounds
void AFlag::PlaySound_Equip_Flag()
{
	if (BaseCharacter)
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			this,
			EquipSound,
			BaseCharacter->GetActorLocation()
		);
	}
}

void AFlag::PlaySound_ScoreFlag()
{
	if (ScoreSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ScoreSound, GetActorLocation());
	}
}

void AFlag::PlaySound_WrongScorePlace(ABaseCharacter* OwnerCharacter)
{
	if (WrongScorePointSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WrongScorePointSound, OwnerCharacter->GetActorLocation());
	}
}
#pragma endregion