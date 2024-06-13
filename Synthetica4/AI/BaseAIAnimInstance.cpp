// ©2023 Reverse-A. All rights reserved.

#include "BaseAIAnimInstance.h"
#include "Synthetica/Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBaseAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	BaseEnemyAI = Cast<ABaseEnemyAI>(TryGetPawnOwner());

}

void UBaseAIAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BaseEnemyAI == nullptr)
	{
		BaseEnemyAI = Cast<ABaseEnemyAI>(TryGetPawnOwner());
	}

	if (BaseEnemyAI)
	{
		MovementSpeed	= BaseEnemyAI->GetVelocity().Size();

		bIsAccelerating = BaseEnemyAI->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0 ? true : false;
		bIsInAir		= BaseEnemyAI->GetCharacterMovement()->IsFalling();
		//bIsCrouched		= BaseEnemyAI->GetCrouch();
		bIsDead			= BaseEnemyAI->GetIsDead();

		DeathValue	    = BaseEnemyAI->GetDeathValue();
		HitReactValue	= BaseEnemyAI->GetHitReactValue();
		bIsHit			= BaseEnemyAI->GetHit();

		bAIRangeInAttackState = BaseEnemyAI->GetRangeAIInAttackState();
	}
}