// @2023 All rights reversed by Reverse-Alpha Studios


#include "AIHumanoidAnimInstance.h"
#include "AIHumanoidModule.h"

void UAIHumanoidAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (TryGetPawnOwner())
	{
		AIEntity = Cast<AAIEntityModule>(TryGetPawnOwner());
	}
}

void UAIHumanoidAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AIEntity = AIEntity == nullptr ? Cast<AAIEntityModule>(TryGetPawnOwner()) : AIEntity;

	if (AIEntity != nullptr)
	{
		AIMovementSpeed = AIEntity->GetVelocity().Size();
	}
}
