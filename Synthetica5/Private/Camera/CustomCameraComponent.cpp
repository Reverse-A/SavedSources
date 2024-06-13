// ©2023 Reverse - A Company.All rights reserved.

#include "Camera/CustomCameraComponent.h"

#include "Character/BaseCharacterMovementComponent.h"
#include "Character/BaseCharacter.h"

UCustomCameraComponent::UCustomCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCustomCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	MouseLookSensitivity = 0.5f;

	BaseCharacter = Cast<ABaseCharacter>(GetOwner());

	if (BaseCharacter)
	{
		BaseCharacterMovementComponent = BaseCharacter->FindComponentByClass<UBaseCharacterMovementComponent>();
	}

}

void UCustomCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	BaseCharacter->CalculateAndClampAimOffset();

	switch (CameraViewPerspective)
	{
	case ECameraViewPerspective::ECVP_FirstPerson:
		FirstPersonCamera();
		break;
	case ECameraViewPerspective::ECVP_ThirdPerson:
		ThirdPersonCamera();
		break;
	}

	if (BaseCharacter)
	{
		AimOffSet_Yaw = BaseCharacter->GetAimOffSetYaw();

		// Booleans
		CharacterIsTurningRight = BaseCharacter->GetTurningRight();
		CharacterIsTurningLeft  = BaseCharacter->GetTurningLeft();
	}
	else { LOG_WARNING("INVALID | BASECHARACTER_class|CustomCamera.cpp"); }
}

#pragma region FPP-TPP
void UCustomCameraComponent::FirstPersonCamera()
{
	float VelocitySize = BaseCharacterMovementComponent->GetCurrentVelocitySize();

	if (VelocitySize > 0.0f)
	{
		BaseCharacter->bUseControllerRotationYaw = true;
	}
	else
	{
		BaseCharacter->bUseControllerRotationYaw = false;
	}

	if (MouseLookSensitivity != 0.3f)
	{
		MouseLookSensitivity = 0.3f;
	}

	if (AimOffSet_Yaw >= 89.9f)
	{
		BaseCharacter->SetTurningRight(true);
		MouseLookSensitivity = 0.0f;
	}

	if (AimOffSet_Yaw <= -89.9f)
	{
		BaseCharacter->SetTurningLeft(true);
		MouseLookSensitivity = 0.0f;
	}

	if (CharacterIsTurningRight || CharacterIsTurningLeft)
	{
		float DeltaSec = GetWorld()->GetDeltaSeconds();

		ResetTurningTimer += DeltaSec;
		if (ResetTurningTimer > 0.5f)
		{
			MouseLookSensitivity = 0.2f;
		}
		if (ResetTurningTimer > 1.0f)
		{
			ResetTurningTimer = 0.f;
			BaseCharacter->SetAimOffSetYawValue(0.0f);
			BaseCharacter->SetTurningRight(false);
			BaseCharacter->SetTurningLeft(false);
		}
	}
}

void UCustomCameraComponent::ThirdPersonCamera()
{
	if (MouseLookSensitivity != 0.5f)
	{
		MouseLookSensitivity = 0.5f;
	}

	if (BaseCharacter->bUseControllerRotationYaw != false)
	{
		BaseCharacter->bUseControllerRotationYaw = false;
	}

	if (BaseCharacter->BaseCharacterMovementComponent->bOrientRotationToMovement != true)
	{
		BaseCharacter->BaseCharacterMovementComponent->bOrientRotationToMovement = true;
	}

}
#pragma endregion

#pragma region ECameraViewPerspective SET_GET_QUEST

void UCustomCameraComponent::SetCameraViewPerspective(ECameraViewPerspective NewPerspective)
{
	CameraViewPerspective = NewPerspective;
}

bool UCustomCameraComponent::IsCameraViewPerspective(ECameraViewPerspective PerspectiveToCheck) const
{
	return CameraViewPerspective == PerspectiveToCheck;
}
#pragma endregion