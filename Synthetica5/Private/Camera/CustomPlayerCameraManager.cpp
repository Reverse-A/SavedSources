// ©2023 Reverse - A Company.All rights reserved.


#include "Camera/CustomPlayerCameraManager.h"

#include "Camera/CameraComponent.h"
#include "Character/BaseCharacter.h"
#include "Character/BaseCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


ACustomPlayerCameraManager::ACustomPlayerCameraManager()
{
	BaseCharacter = Cast<ABaseCharacter>(GetOwner());
}

void ACustomPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (BaseCharacter)
	{	
		UBaseCharacterMovementComponent* BCMovComp = BaseCharacter->BaseCharacterMovementComponent;

		FVector TargetCrouchOffset = FVector(
			0,
			0,
			BCMovComp->GetCrouchedHalfHeight() - BaseCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		);
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));


		if (BCMovComp->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		OutVT.POV.Location += Offset;
	}
}
