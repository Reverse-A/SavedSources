// ©2023 Reverse - A Company. All rights reserved.


#include "Camera/CameraSettings.h"

#include "Animation/CoreAnimProcessor.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Camera/PlayerCameraManager.h"
#include "Character/BaseCharacter.h"
#include "Components/EnhancedMovementComponent.h"
#include "Components/CombatComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerManager.h"
#include "Weapons/Weapon.h"


#include "Net/UnrealNetwork.h"


UCameraSettings::UCameraSettings()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraSettings::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCameraSettings, CameraViewPerspective);
}


void UCameraSettings::BeginPlay()
{
	Super::BeginPlay();

	BaseCharacter = Cast<ABaseCharacter>(GetOwner());
	if (BaseCharacter)
	{
		EnhancedMovement = BaseCharacter->FindComponentByClass<UEnhancedMovementComponent>();
		CombatComponent  = BaseCharacter->FindComponentByClass<UCombatComponent>();

		if (BaseCharacter->GetThirdPersonCamera())
		{
			DefaultFOV = BaseCharacter->GetThirdPersonCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
	}
	SetCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson);
	CoreAnimProcessor = Cast<UCoreAnimProcessor>(BaseCharacter->Head->GetAnimInstance());
}


void UCameraSettings::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CameraSystemOperation();
	SetHUDCrosshairs(DeltaTime);
}


void UCameraSettings::CameraSystemOperation()
{
	if (BaseCharacter)
	{
		CameraTick();
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
}

void UCameraSettings::CrouchCameraInterpolation()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (EnhancedMovement->IsCrouching())
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
			float InterpolationSpeed = 10.0f;

			FVector CurrentCameraLocation = BaseCharacter->GetThirdPersonCamera()->GetRelativeLocation();
			FVector NewCameraLocation = FMath::VInterpTo(CurrentCameraLocation, TargetUnCrouchLocation, DeltaTime, InterpolationSpeed);

			BaseCharacter->GetThirdPersonCamera()->SetRelativeLocation(NewCameraLocation);
		}
	}
}

void UCameraSettings::SwitchCamera()
{
	if (bIsCameraSwitchOnCooldown) return;

	if (BaseCharacter->ThirdPersonCamera->IsActive())
	{
		SwitchToFirstPersonCamera();
	}
	else
	{
		SwitchToThirdPersonCamera();
	}

	GetWorld()->GetTimerManager().SetTimer
	(
		TimerHandle_CameraSwitchCooldown,
		this,
		&UCameraSettings::ResetCameraSwitchCooldown,
		CameraSwitchCooldown, false
	);
	bIsCameraSwitchOnCooldown = true;
}

void UCameraSettings::ResetCameraSwitchCooldown()
{
	bIsCameraSwitchOnCooldown = false;
}

void UCameraSettings::InterpFOV(float DeltaTime)
{
	if (BaseCharacter->GetEquippedWeapon() == nullptr)
	{
		return;
	}

	if (BaseCharacter->IsAiming())
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, BaseCharacter->GetEquippedWeapon()->GetZoomedFOV(), DeltaTime, BaseCharacter->GetEquippedWeapon()->GetZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}

	if (BaseCharacter && BaseCharacter->GetThirdPersonCamera())
	{
		BaseCharacter->GetThirdPersonCamera()->SetFieldOfView(CurrentFOV);
	}
}



void UCameraSettings::SwitchToFirstPersonCamera()
{
	if (BaseCharacter->HasAuthority())
	{
		SetCameraViewPerspective(ECameraViewPerspective::ECVP_FirstPerson);
	}
	else
	{
		ServerSetCameraViewPerspective(ECameraViewPerspective::ECVP_FirstPerson);
	}

	BaseCharacter->ThirdPersonCamera->SetActive(false);
	BaseCharacter->FirstPersonCamera->SetActive(true);

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

void UCameraSettings::SwitchToThirdPersonCamera()
{
	if (BaseCharacter->HasAuthority())
	{
		SetCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson);
	}
	else
	{
		ServerSetCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson);
	}

	BaseCharacter->FirstPersonCamera->SetActive(false);
	BaseCharacter->ThirdPersonCamera->SetActive(true);

	BaseCharacter->Hair->SetVisibility(true);
	BaseCharacter->Head->SetVisibility(true);
}

void UCameraSettings::PlaySwitchCameraAnimation(bool bChange)
{
	if (bChange)
	{
		if (IsCameraViewPerspective(ECameraViewPerspective::ECVP_FirstPerson))
		{
			FVector ThirdPersonCameraLocation = BaseCharacter->ThirdPersonCamera->GetRelativeLocation();
			FVector FirstPersonCameraLocation = BaseCharacter->FirstPersonCamera->GetRelativeLocation();
			if (CameraLocationLerpCurve)
			{
				float CurrentTime			= 1.0f;
				float InterpolationFactor	= CameraLocationLerpCurve->GetFloatValue(CurrentTime);
				FVector LerpedLocation		= FMath::Lerp(ThirdPersonCameraLocation, FirstPersonCameraLocation, InterpolationFactor);
				BaseCharacter->ThirdPersonCamera->SetRelativeLocation(LerpedLocation);
			}
		}
		else if (IsCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson))
		{
			FVector FirstPersonCameraLocation = BaseCharacter->FirstPersonCamera->GetRelativeLocation();
			FVector ThirdPersonCameraLocation = BaseCharacter->ThirdPersonCamera->GetRelativeLocation();

			if (CameraLocationLerpCurve)
			{
				float CurrentTime = 1.0f;
				float InterpolationFactor = CameraLocationLerpCurve->GetFloatValue(CurrentTime);
				FVector LerpedLocation = FMath::Lerp(FirstPersonCameraLocation, ThirdPersonCameraLocation, InterpolationFactor);
				BaseCharacter->ThirdPersonCamera->SetRelativeLocation(LerpedLocation);
			}
		}
		bChange = false;
	}
}

#pragma region | FPP-TPP |
void UCameraSettings::FirstPersonCamera()
{
	if (BaseCharacter->IsCharacterDead())
	{
		// Set Camera last location to freeze camera location
	}
	else
	{
		// Set Camera location as determined at begining.
	}
	if (MouseLookSensitivity != 0.3f)
	{
		MouseLookSensitivity = 0.3f;
	}
	float VelocitySize = EnhancedMovement->Velocity.Size();

	if (BaseCharacter->IsAiming())
	{
		BaseCharacter->bUseControllerRotationYaw = false;
		EnhancedMovement->bOrientRotationToMovement = false;
	}
	else
	{
		if (BaseCharacter->GetMovementDirectionY() != 0)
		{
			BaseCharacter->bUseControllerRotationYaw = true;
			EnhancedMovement->bOrientRotationToMovement = true;
		}
		else
		{
			BaseCharacter->bUseControllerRotationYaw = false;
			EnhancedMovement->bOrientRotationToMovement = false;
		}

	}
}

void UCameraSettings::ThirdPersonCamera()
{
	if (BaseCharacter->IsCharacterDead())
	{
		// Set Camera last location to freeze camera location
	}
	else
	{

	}

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	InterpFOV(DeltaTime);

	HideCameraIfCharacterClose();

	if (BaseCharacter->IsAiming())
	{
		if (MouseLookSensitivity != 0.2f)
		{
			MouseLookSensitivity = 0.2f;
		}

		if (EnhancedMovement->GetVelocitySize() == 0.0f)
		{
			if (EnhancedMovement->bOrientRotationToMovement != false)
			{
				EnhancedMovement->bOrientRotationToMovement = false;
			}
			if (CoreAnimProcessor->IsCharacterRotation())
			{
				if (BaseCharacter->bUseControllerRotationYaw != true)
				{
					BaseCharacter->bUseControllerRotationYaw = true;
				}
			}
			else
			{
				if (BaseCharacter->bUseControllerRotationYaw != false)
				{
					BaseCharacter->bUseControllerRotationYaw = false;
				}
			}
		}
		else
		{
			if (BaseCharacter->bUseControllerRotationYaw != true)
			{
				BaseCharacter->bUseControllerRotationYaw = true;
			}
			if (EnhancedMovement->bOrientRotationToMovement != false)
			{
				EnhancedMovement->bOrientRotationToMovement = false;
			}
		}
	}
	else
	{
		if (MouseLookSensitivity != 0.5f)
		{
			MouseLookSensitivity = 0.5f;
		}

		if (BaseCharacter->bUseControllerRotationYaw != false)
		{
			BaseCharacter->bUseControllerRotationYaw = false;
		}

		if (EnhancedMovement->bOrientRotationToMovement != true)
		{
			EnhancedMovement->bOrientRotationToMovement = true;
		}
	}
}

void UCameraSettings::HideCameraIfCharacterClose()
{
	if (BaseCharacter->GetThirdPersonCamera())
	{
		if ((BaseCharacter->GetThirdPersonCamera()->GetComponentLocation() - BaseCharacter->GetActorLocation()).Size() < HideCameraThreshold)
		{
			if (bMeshesVisible)
			{
				CharacterMeshesVisibility(false);

				if (CombatComponent)
				{
					if (CombatComponent->GetEquippedWeapon() != nullptr)
					{
						if (CombatComponent->GetEquippedWeapon()->GetWeaponMesh())
						{
							CombatComponent->GetEquippedWeapon()->GetWeaponMesh()->bOwnerNoSee = true;
						}
					}
					else
					{

					}
				}
				else
				{
					LOG_WARNING("CombatComponent NULL");
				}
			}
			bMeshesVisible = false;
		}
		else
		{
			if (!bMeshesVisible)
			{
				CharacterMeshesVisibility(true);
				if (CombatComponent)
				{
					if (CombatComponent->GetEquippedWeapon() != nullptr)
					{
						if (CombatComponent->GetEquippedWeapon()->GetWeaponMesh())
						{
							CombatComponent->GetEquippedWeapon()->GetWeaponMesh()->bOwnerNoSee = false;
						}
					}
				}
				else
				{
					LOG_WARNING("CombatComponent NULL");
				}
				bMeshesVisible = true;
			}
		}
	}
	else
	{
		LOG_WARNING("GetThirdPersonCamera NULL");
	}
}


void UCameraSettings::CharacterMeshesVisibility(bool Visibility)
{
	if (BaseCharacter->IsLocallyControlled() && IsCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson))
	{
		BaseCharacter->GetHairMesh()->SetVisibility(Visibility);
		BaseCharacter->GetHeadMesh()->SetVisibility(Visibility);
		BaseCharacter->GetTorsoMesh()->SetVisibility(Visibility);
		BaseCharacter->GetLegsMesh()->SetVisibility(Visibility);
		BaseCharacter->GetArmsMesh()->SetVisibility(Visibility);
		BaseCharacter->GetHandsMesh()->SetVisibility(Visibility);
		BaseCharacter->GetBagMesh()->SetVisibility(Visibility);
	}
}
#pragma endregion

#pragma region | Trace&&Crosshair System |
void UCameraSettings::SetHUDCrosshairs(float DeltaTime)
{
	if (BaseCharacter == nullptr || BaseCharacter->Controller == nullptr)
	{
		return;
	}

	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

	if (PlayerControllerManager)
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(PlayerControllerManager->GetHUD()) : PlayerHUD;

		if (PlayerHUD)
		{
			if (BaseCharacter->GetEquippedWeapon())
			{

				if (EnhancedMovement->IsSprinting() && EnhancedMovement->GetVelocitySize() >= 700.f)
				{
					HUDPackage.CrosshairsCenter = CrosshairsCenter;
					HUDPackage.CrosshairsLeft	= nullptr;
					HUDPackage.CrosshairsRight	= nullptr;
					HUDPackage.CrosshairsTop	= nullptr;
					HUDPackage.CrosshairsBottom = nullptr;
				}

				if (BaseCharacter->GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_Shotgun)
				{
					HUDPackage.CrosshairsCenter = CircleTypeCrosshairsCenter;
					HUDPackage.CrosshairsLeft	= CircleTypeCrosshairsLeft;
					HUDPackage.CrosshairsRight  = CircleTypeCrosshairsRight;
					HUDPackage.CrosshairsTop	= nullptr;
					HUDPackage.CrosshairsBottom = nullptr;
				}
				else
				{
					HUDPackage.CrosshairsCenter = CrosshairsCenter;
					HUDPackage.CrosshairsLeft	= CrosshairsLeft;
					HUDPackage.CrosshairsRight	= CrosshairsRight;
					HUDPackage.CrosshairsTop	= CrosshairsTop;
					HUDPackage.CrosshairsBottom = CrosshairsBottom;
				}
			}
			else
			{
				HUDPackage.CrosshairsCenter		= CrosshairsCenter;
				HUDPackage.CrosshairsLeft		= nullptr;
				HUDPackage.CrosshairsRight		= nullptr;
				HUDPackage.CrosshairsTop		= nullptr;
				HUDPackage.CrosshairsBottom		= nullptr;
			}
			/*Calculate crosshair's spread*/
			FVector2D WalkSpeedRange(0.f, EnhancedMovement->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.0f, 1.0f);

			FVector Velocity = BaseCharacter->GetVelocity();
			Velocity.Z = 0.0f;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if (EnhancedMovement->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
			}

			if (BaseCharacter->IsAiming())
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.58f, DeltaTime, 20.0f);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 20.0f);
			}
			const float NeutralSpreadSize = 0.15f;
			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.f);


			HUDPackage.CrosshairSpread =
				NeutralSpreadSize +
				CrosshairVelocityFactor +
				CrosshairInAirFactor -
				CrosshairAimFactor +
				CrosshairShootingFactor;

			PlayerHUD->SetHUDPackage(HUDPackage);
		}
	}
}
#pragma endregion

#pragma region Setters - Getters
void UCameraSettings::SetCameraViewPerspective(ECameraViewPerspective NewPerspective)
{
	CameraViewPerspective = NewPerspective;
}

void UCameraSettings::ServerSetCameraViewPerspective_Implementation(ECameraViewPerspective NewPerspective)
{
	if (BaseCharacter->HasAuthority())
	{
		CameraViewPerspective = NewPerspective;
	}
	else
	{
		MulticastSetCameraViewPerspective(NewPerspective);
	}
}

void UCameraSettings::MulticastSetCameraViewPerspective_Implementation(ECameraViewPerspective NewPerspective)
{
	CameraViewPerspective = NewPerspective;
}

bool UCameraSettings::ServerSetCameraViewPerspective_Validate(ECameraViewPerspective NewPerspective)
{
	return true;
}

bool UCameraSettings::IsCameraViewPerspective(ECameraViewPerspective PerspectiveToCheck) const
{
	return CameraViewPerspective == PerspectiveToCheck;
}
#pragma endregion


#pragma region CameraTilt
void UCameraSettings::CameraTick()
{

	if (!EnhancedMovement->IsWallRunning())
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		CameraTiltValue = FMath::FInterpConstantTo(CameraTiltValue, 0.0f, DeltaTime, 15.0f);
		CameraTilt(CameraTiltValue);

		if (!WallRun_CameraShakeCanPlayableAgain)
		{
			WallRun_CameraShakeCanPlayableAgain = true;
		}
	}

	switch (EnhancedMovement->CustomMovementMode)
	{
	case CMOVE_WallRun:

		float DeltaTime = GetWorld()->GetDeltaSeconds();
		if (EnhancedMovement->WallRunningIsRight())
		{
			CameraTiltValue = FMath::FInterpConstantTo(CameraTiltValue, -15.0f, DeltaTime, 10.f);
			CameraTilt(CameraTiltValue);
			Client_WallRunCameraShake();
		}
		else
		{
			CameraTiltValue = FMath::FInterpConstantTo(CameraTiltValue, 15.0f, DeltaTime, 10.f);
			CameraTilt(CameraTiltValue);
			Client_WallRunCameraShake();
		}
		break;
	}
}

void UCameraSettings::CameraTilt(float TargetXRoll)
{
	if (BaseCharacter->IsLocallyControlled())
	{
		if (!EnhancedMovement->IsWallRunning() && TargetXRoll == 0)
		{
			return;
		}

		AController* CharacterController = BaseCharacter->GetController();

		if (CharacterController)
		{
			FRotator ControlRotation = CharacterController->GetControlRotation();
			float ControlRotationPitch = ControlRotation.Pitch;
			float ControlRotationYaw = ControlRotation.Yaw;
			float DeltaSecond = GetWorld()->GetDeltaSeconds();
			float InterpSpeed = 10.0f;
			FRotator NewRotation = FMath::RInterpTo(ControlRotation, FRotator(ControlRotationPitch, ControlRotationYaw, TargetXRoll), DeltaSecond, InterpSpeed);
			CharacterController->SetControlRotation(NewRotation);

		}
		else
		{
			LOG_WARNING("CONTROLLER IS NULL!");
		}
	}
}

void UCameraSettings::Client_ClimbCameraShake_Implementation()
{
	if (BaseCharacter->IsLocallyControlled())
	{
		APlayerCameraManager* PlayerCameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
		if (PlayerCameraManager)
		{
			if (Climb_CameraShake)
			{
				PlayerCameraManager->StartCameraShake(Climb_CameraShake, 1);
			}
		}
	}
}

void UCameraSettings::Client_WallRunCameraShake_Implementation()
{
	if (BaseCharacter->IsLocallyControlled())
	{
		if (WallRun_CameraShakeCanPlayableAgain)
		{
			APlayerCameraManager* PlayerCameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
			if (PlayerCameraManager)
			{
				if (Wallrun_CameraShake)
				{
					PlayerCameraManager->StartCameraShake(Wallrun_CameraShake, 1);
					WallRun_CameraShakeCanPlayableAgain = false;
				}
				else
				{
					LOG_WARNING("WALLRUN NULL");
				}
			}
			else
			{
				LOG_WARNING("PLAYERCAMERA NULL");
			}
		}
	}
}
#pragma endregion