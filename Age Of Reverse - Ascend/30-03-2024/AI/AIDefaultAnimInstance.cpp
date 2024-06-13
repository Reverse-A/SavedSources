// @2023 All rights reversed by Reverse-Alpha Studios


#include "AIDefaultAnimInstance.h"
#include "AIEntityModule.h"

void UAIDefaultAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (TryGetPawnOwner())
	{
		if (TryGetPawnOwner()->IsPlayerControlled()) return;

		AIEntity = Cast<AAIEntityModule>(TryGetPawnOwner());
	}
	
}

void UAIDefaultAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AIEntity = AIEntity == nullptr ? Cast<AAIEntityModule>(TryGetPawnOwner()) : AIEntity;

	if (AIEntity != nullptr)
	{
		PawnSpeed = AIEntity->GetVelocity().Size();
	}
}
