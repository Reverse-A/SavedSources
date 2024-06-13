// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|


#include "Camera/CameraConfiguration.h"

#include "Camera/CameraComponent.h"
#include "Characters/BaseCharacter.h"
#include "Components/EnhancedMovementComponent.h"


UCameraConfiguration::UCameraConfiguration()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson);
}


void UCameraConfiguration::BeginPlay()
{
	Super::BeginPlay();
	BaseCharacter = Cast<ABaseCharacter>(GetOwner());
	if (BaseCharacter)
	{
		EnhancedMovementComponent = BaseCharacter->FindComponentByClass<UEnhancedMovementComponent>();
	}
}


void UCameraConfiguration::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GeneralCameraSystemFunctionOperation();
}

void UCameraConfiguration::GeneralCameraSystemFunctionOperation()
{
	if (BaseCharacter == nullptr) return;
	CrouchCameraInterpolation();

	switch (CameraViewPerspective)
	{
	case ECameraViewPerspective::ECVP_FirstPerson:
		FirstPersonCamera();
		break;
	case ECameraViewPerspective::ECVP_ThirdPerson:
		ThirdPersonCamera();
		break;
	}
}

#pragma region CameraSwitch
void UCameraConfiguration::SwitchCamera()
{
	if (bCameraSwitchCooldown || BaseCharacter == nullptr) return;

	if (BaseCharacter->GetThirdPersonCamera()->IsActive())
	{
		SwitchToFirstPersonCamera();
	}
	else
	{
		SwitchToThirdPersonCamera();
	}

	GetWorld()->GetTimerManager().SetTimer
	(
		CameraSwitchCooldownTimerHandle,
		this,
		&UCameraConfiguration::ResetCameraSwitchCooldown,
		CameraSwitchCooldownTime,
		false
	);

	bCameraSwitchCooldown = true;
}

void UCameraConfiguration::SwitchToFirstPersonCamera()
{
	SetCameraViewPerspective(ECameraViewPerspective::ECVP_FirstPerson);

	BaseCharacter->GetThirdPersonCamera()->SetActive(false);
	BaseCharacter->GetFirstPersonCamera()->SetActive(true);

	if (BaseCharacter->IsLocallyControlled())
	{
		BaseCharacter->Hair->SetVisibility(false);
		BaseCharacter->Head->SetVisibility(false);
	}
	else
	{
		BaseCharacter->Hair->SetVisibility(true);
		BaseCharacter->Head->SetVisibility(true);
	}
}

void UCameraConfiguration::SwitchToThirdPersonCamera()
{
	SetCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson);

	BaseCharacter->GetFirstPersonCamera()->SetActive(false);
	BaseCharacter->GetThirdPersonCamera()->SetActive(true);

	BaseCharacter->Hair->SetVisibility(true);
	BaseCharacter->Head->SetVisibility(true);
}

void UCameraConfiguration::ResetCameraSwitchCooldown()
{
	bCameraSwitchCooldown = false;
}

void UCameraConfiguration::FirstPersonCamera()
{
	if (BaseCharacter->GetMouseSensitivityValue() != 0.3f)
		BaseCharacter->SetMouseSensitivityValue(0.3f);
}

void UCameraConfiguration::ThirdPersonCamera()
{
	float Velocity = EnhancedMovementComponent->Velocity.Size();

	if (BaseCharacter->GetMouseSensitivityValue() != 0.2f)
		BaseCharacter->SetMouseSensitivityValue(0.2f);
}
void UCameraConfiguration::CrouchCameraInterpolation()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (EnhancedMovementComponent->IsCrouching())
	{
		FVector TargetCrouchLocation = FVector(0.0f, 0.0f, -60.0f);
		if (BaseCharacter->GetThirdPersonCamera()->GetRelativeLocation() != TargetCrouchLocation)
		{
			float InterpolationSpeed = 10.0f;

			FVector CurrentCameraLocation = BaseCharacter->GetThirdPersonCamera()->GetRelativeLocation();
			FVector NewCameraLocation = FMath::VInterpTo(CurrentCameraLocation, TargetCrouchLocation, DeltaTime, InterpolationSpeed);

			BaseCharacter->GetThirdPersonCamera()->SetRelativeLocation(NewCameraLocation);
		}
	}
	else
	{
		FVector TargetUnCrouchLocation = FVector(0.0f, 0.0f, 0.0f);

		if (BaseCharacter->GetThirdPersonCamera()->GetRelativeLocation() != TargetUnCrouchLocation)
		{
			float InterpolationSpeed = 1.5f;

			FVector CurrentCameraLocation = BaseCharacter->GetThirdPersonCamera()->GetRelativeLocation();
			FVector NewCameraLocation = FMath::VInterpTo(CurrentCameraLocation, TargetUnCrouchLocation, DeltaTime, InterpolationSpeed);

			BaseCharacter->GetThirdPersonCamera()->SetRelativeLocation(NewCameraLocation);
		}
	}
}
#pragma endregion


bool UCameraConfiguration::IsCameraViewPerspective(ECameraViewPerspective PerspectiveToCheck) const
{
	return CameraViewPerspective == PerspectiveToCheck;
}