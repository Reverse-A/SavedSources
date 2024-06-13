// ©2023 Reverse-A. All rights reserved.


#include "MeleeEnemyAI.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Synthetica/Weapons/Projectile.h"
#include "Synthetica/Weapons/MeleeWeapons/BaseMeleeWeapon.h"

void AMeleeEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	bWeaponAttached = false;

}

void AMeleeEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsDead)StateConditions(DeltaTime);

}

void AMeleeEnemyAI::StateConditions(float DeltaTime)
{
	switch (AIState)
	{
	case EAIState::EAIS_Idle:
		// Check conditions for the idle state
		AI_Melee_IdleConditions(); 
		// If a melee weapon is attached, detach it
		if(bWeaponAttached)DetachWeapon();
		break;

	case EAIState::EAIS_Patrolling:
		// Check conditions for the patrolling state
		AI_Melee_PatrolConditions(DeltaTime);
		break;

	case EAIState::EAIS_ChaseState:

		// If no melee weapon is attached, attach it
		if (!bWeaponAttached)AttachWeapon();

		// If in melee attack distance, transition to the attack state
		if (bInMeleeAttackDistance)
		{
			AIState = EAIState::EAIS_AttackState;
		}
		break;

	case EAIState::EAIS_AttackState:
		// Perform actions for the attack state
		AI_Melee_AttackState();
		break;
	}
}

void AMeleeEnemyAI::AI_Melee_IdleConditions()
{

	if (bPatrolSpeechIsTriggered)
	{
		bPatrolSpeechIsTriggered = false;
		PatrolSpeechRandomTimerDelay = 0.f;
	}

}

void AMeleeEnemyAI::AI_Melee_PatrolConditions(float DeltaTime)
{
	PatrolSpeechRandomTimerDelay += DeltaTime;
	const float RandomNum = FMath::RandRange(1.f, 3.f);
	if (PatrolSpeechRandomTimerDelay > RandomNum)
	{
		if (!bPatrolSpeechIsTriggered)
		{
			PlayRandomPatrolSpeech();
			bPatrolSpeechIsTriggered = true;
		}
	}	
}

void AMeleeEnemyAI::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AMeleeEnemyAI::PlayRandomPatrolSpeech()
{
	int32 RandomIndex = FMath::RandRange(0, PatrolSpeechSounds.Num() - 1);
	USoundCue* SelectedSoundCue = PatrolSpeechSounds[RandomIndex];

	if (SelectedSoundCue) UGameplayStatics::PlaySoundAtLocation(this, SelectedSoundCue, GetActorLocation());
}

void AMeleeEnemyAI::AI_Melee_AttackState() 
{
	const float AttackRadiusTimerDelay = 1.5f;
	if (bInMeleeAttackDistance && bCombatTarget_IsDetected)
	{
		bIsChasing = false;
		if (bCanMeleeAttack)
		{
			MeleeAttack();
			bCanMeleeAttack = false;
		}
	}
}

void AMeleeEnemyAI::MeleeAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontages)
	{
		TArray<FName> SectionNames = { FName("SlideAttack"), FName("SpinAttack"), FName("SlashAttack") };
		int32 NumSections = SectionNames.Num();
		int32 RandomIndex = FMath::RandRange(0, NumSections - 1);

		FName RandomSectionName = SectionNames[RandomIndex];

		// Play the random section of the attack montage
		AnimInstance->Montage_Play(AttackMontages, 1.0f);
		AnimInstance->Montage_JumpToSection(RandomSectionName);
	}
}