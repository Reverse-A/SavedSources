// ©2023 Reverse-A. All rights reserved.


#include "RangeEnemyAI.h"
#include "Synthetica/Weapons/Weapon.h"
#include "TimerManager.h"

void ARangeEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	if(bWeaponAttached == true)		bWeaponAttached = false;
	if (bRangeInAttackState == true) bRangeInAttackState = false;
}

void ARangeEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsDead)AI_Range_GeneralConditions(DeltaTime);
}

void ARangeEnemyAI::AI_Range_GeneralConditions(float DeltaTime)
{
	switch (AIState)
	{
	case EAIState::EAIS_Idle:
		// Check conditions for the idle state
		AI_Range_IdleConditions();
		// If a melee weapon is attached, detach it
		if (bWeaponAttached)DetachWeapon();
		break;

	case EAIState::EAIS_Patrolling:
		// Check conditions for the patrolling state
		AI_Range_PatrolConditions(DeltaTime);
		break;
	case EAIState::EAIS_AlertState:
		AI_Range_AlertState();
		break;
	case EAIState::EAIS_ChaseState:

		// If no melee weapon is attached, attach it
		if (!bWeaponAttached)AttachWeapon();

		// If in melee attack distance, transition to the attack state
		if (bInRangeAttackDistance)
		{
			if (!bRangeInAttackState) bRangeInAttackState;

			if(AIState != EAIState::EAIS_AttackState)
				AIState = EAIState::EAIS_AttackState;
		}
		break;
	case EAIState::EAIS_AttackState:
		// Perform actions for the attack state
		AI_Range_AttackState();
		break;
	}
}


void ARangeEnemyAI::AI_Range_IdleConditions()
{
	if (bRangeInAttackState) bRangeInAttackState = false;

}

void ARangeEnemyAI::AI_Range_PatrolConditions(float DeltaTime)
{
	if (bRangeInAttackState) bRangeInAttackState = false;
}

void ARangeEnemyAI::AI_Range_AlertState()
{
	if (bRangeInAttackState) bRangeInAttackState = false;
}

void ARangeEnemyAI::AI_Range_AttackState()
{
	EnemyAIController->StopMovement();
	if (!bRangeInAttackState) bRangeInAttackState = true;
	LookAtTarget();
	if(bCanRangeFire)StartFireTimer_AIRange();

	if (!bInCombatDistance)
	{
		if (AIState != EAIState::EAIS_AlertState)
			AIState = EAIState::EAIS_AlertState;
	}

	if (!bInRangeAttackDistance && bInCombatDistance)
	{
		if(AIState != EAIState::EAIS_ChaseState)
			AIState = EAIState::EAIS_ChaseState;
	}
}


void ARangeEnemyAI::LookAtTarget()
{
	if (CombatTarget)
	{
		// Get the CombatTarget's location
		FVector TargetLocation = CombatTarget->GetActorLocation();

		// Get the direction to the target
		FVector LookAtDirection = TargetLocation - GetActorLocation();
		LookAtDirection.Normalize();

		// Calculate the new rotation for the character
		FRotator NewRotation = LookAtDirection.Rotation();
		SetActorRotation(NewRotation);
	}
}


void ARangeEnemyAI::StartFireTimer_AIRange()
{
	bCanRangeFire = false;
	GetWorldTimerManager().SetTimer(
		AIRange_FireTimer,
		this,
		&ARangeEnemyAI::Fire_AIRange,
		FMath::RandRange(minFireDelay, maxFireDelay)
	);
}

void ARangeEnemyAI::Fire_AIRange()
{
	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();

		// Adjust these values according to your requirements
		const float MaxRandomOffsetFactor = 0.01f; // Maximum random offset factor
		const float MaxDistanceToPlayer = 1500.0f; // Maximum distance to the player

		// Calculate the maximum random offset based on the player's maximum distance and character's capsule dimensions

		float MaxRandomOffset = MaxDistanceToPlayer * MaxRandomOffsetFactor;

		float AvarageCapsuleRadius = 34.0f;
		float AvarageCapsuleHalfHeight = 88.0;

		MaxRandomOffset += FVector(AvarageCapsuleRadius, AvarageCapsuleRadius, AvarageCapsuleHalfHeight).Size();


		FVector RandomOffset = FVector(FMath::FRandRange(-MaxRandomOffset, MaxRandomOffset),
									   FMath::FRandRange(-MaxRandomOffset, MaxRandomOffset),
									   FMath::FRandRange(-MaxRandomOffset, MaxRandomOffset));
		CombatTargetLocation += RandomOffset;



		RangeEquippedWeapon->Fire(CombatTargetLocation);	
		bCanRangeFire = true;
	}
}