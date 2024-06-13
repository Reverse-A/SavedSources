// @2023 All rights reversed by Reverse-Alpha Studios


#include "Camera/CameraManager.h"

#include "Camera/CameraComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/SynthCharacter.h"
#include "Components/EnhancedMovementComponent.h"
#include "Components/CombatComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/PlayerHUD.h"
#include "PlayerController/PlayerControllerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Weapon.h"

UCameraManager::UCameraManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraManager::BeginPlay()
{
	Super::BeginPlay();
	BaseCharacter = Cast<ABaseCharacter>(GetOwner());

	if (BaseCharacter)
	{
		EnhancedMovementComponent	= BaseCharacter->FindComponentByClass<UEnhancedMovementComponent>();
		CombatComponent				= BaseCharacter->FindComponentByClass<UCombatComponent>();

		if (BaseCharacter->GetThirdPersonCamera())
		{
			DefaultFOV		= BaseCharacter->GetThirdPersonCamera()->FieldOfView;
			CurrentFOV		= DefaultFOV;
			bActorVisible	= true;
		}
	}
}

void UCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (BaseCharacter == nullptr) return;
	if (BaseCharacter && BaseCharacter->IsLocallyControlled())
	{	
		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
		Crouch_InterpolateCameraPosition(DeltaTime);
		HideActor();
	}
	SprintFOV(DeltaTime);
}

void UCameraManager::HideActor()
{
	if (BaseCharacter == nullptr || BaseCharacter->CharacterIsDead()) return;
	// Note : In the future, when the first person camera is activated, it can switch to first person camera mode.  
	if (BaseCharacter->GetThirdPersonCamera())
	{
		float HideCharacterDistance = 200.0f;

		if ((BaseCharacter->GetThirdPersonCamera()->GetComponentLocation() - BaseCharacter->GetActorLocation()).Size() < HideCharacterDistance)
		{
			if (bActorVisible)
			{
				SetActorMeshesVisibility(false);
				if (BaseCharacter->GetEquippedWeapon() != nullptr)
				{
					BaseCharacter->GetEquippedWeapon()->GetWeaponMesh()->bOwnerNoSee = true;
				}
				bActorVisible = false;
			}
		}
		else
		{
			if (!bActorVisible)
			{
				SetActorMeshesVisibility(true);
				if (BaseCharacter->GetEquippedWeapon() != nullptr)
				{
					BaseCharacter->GetEquippedWeapon()->GetWeaponMesh()->bOwnerNoSee = false;
				}
				bActorVisible = true;
			}

		}
	}
}

void UCameraManager::SetActorMeshesVisibility(bool bVisible)
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetOwner()) : BaseCharacter;
	if (BaseCharacter && BaseCharacter->IsLocallyControlled())
	{
		BaseCharacter->GetMesh()	  ->SetVisibility(bVisible);
		BaseCharacter->GetHairMesh()  ->SetVisibility(bVisible);
		BaseCharacter->GetHeadMesh()  ->SetVisibility(bVisible);
		BaseCharacter->GetLegsMesh()  ->SetVisibility(bVisible);
		BaseCharacter->GetArmsMesh()  ->SetVisibility(bVisible);
		BaseCharacter->GetHandsMesh() ->SetVisibility(bVisible);
		ASynthCharacter* SynthCharacter = Cast<ASynthCharacter>(BaseCharacter);
		if (SynthCharacter)
		{
			SynthCharacter->GetBagMesh()->SetVisibility(bVisible);
		}

		CombatComponent = CombatComponent == nullptr ? BaseCharacter->FindComponentByClass<UCombatComponent>() : CombatComponent;
		if (CombatComponent && CombatComponent->GetSecondaryWeapon() && CombatComponent->GetSecondaryWeapon()->GetWeaponMesh())
		{
			CombatComponent->GetSecondaryWeapon()->GetWeaponMesh()->SetVisibility(bVisible);
		}
	}
}

// This functions is not using right now// 
void UCameraManager::InitialCrosshair()
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetOwner()) : BaseCharacter;

	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(PlayerControllerManager->GetHUD()) : PlayerHUD;
			if (PlayerHUD)
			{

				HUDPackage.CrosshairsCenter = CrosshairsCenter;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
			}
		}
	}
}

void UCameraManager::SetHUDCrosshairs(float DeltaTime)
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetOwner()) : BaseCharacter;
	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

	if (PlayerControllerManager)
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(PlayerControllerManager->GetHUD()) : PlayerHUD;
		if (PlayerHUD)
		{
			if (BaseCharacter->GetEquippedWeapon() == nullptr)
			{
				HUDPackage.CrosshairsCenter = CrosshairsCenter;
				HUDPackage.CrosshairsLeft	= nullptr;
				HUDPackage.CrosshairsRight	= nullptr;
				HUDPackage.CrosshairsTop	= nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
			}
			else
			{
				if (BaseCharacter->GetEquippedWeapon() == nullptr) return;

				if (EnhancedMovementComponent->IsSprinting() && EnhancedMovementComponent->GetVelocitySize() >= 700.0f)
				{
					HUDPackage.CrosshairsCenter = CrosshairsCenter;
					HUDPackage.CrosshairsLeft	= nullptr;
					HUDPackage.CrosshairsRight	= nullptr;
					HUDPackage.CrosshairsTop	= nullptr;
					HUDPackage.CrosshairsBottom = nullptr;
				}
				else
				{
					if (BaseCharacter->GetEquippedWeapon()->Get_WeaponType() == EWeaponType::EWT_Pistol)
					{
						HUDPackage.CrosshairsCenter = CrosshairsCenter;
						HUDPackage.CrosshairsLeft   = nullptr;
						HUDPackage.CrosshairsRight  = nullptr;
						HUDPackage.CrosshairsTop	= nullptr;
						HUDPackage.CrosshairsBottom = nullptr;
					}
					else if (BaseCharacter->GetEquippedWeapon()->Get_WeaponType() == EWeaponType::EWT_SubmachineGun)
					{
						HUDPackage.CrosshairsCenter = CrosshairsCenter;
						HUDPackage.CrosshairsLeft   = CrosshairsLeft;
						HUDPackage.CrosshairsRight  = CrosshairsRight;
						HUDPackage.CrosshairsTop    = CrosshairsTop;
						HUDPackage.CrosshairsBottom = CrosshairsBottom;
					}
					else if (BaseCharacter->GetEquippedWeapon()->Get_WeaponType() == EWeaponType::EWT_AssaultRifle)
					{
						HUDPackage.CrosshairsCenter = CrosshairsCenter;
						HUDPackage.CrosshairsLeft	= CrosshairsLeft;
						HUDPackage.CrosshairsRight	= CrosshairsRight;
						HUDPackage.CrosshairsTop	= CrosshairsTop;
						HUDPackage.CrosshairsBottom = CrosshairsBottom;
					}
					else if (BaseCharacter->GetEquippedWeapon()->Get_WeaponType() == EWeaponType::EWT_RocketLauncher)
					{
						HUDPackage.CrosshairsCenter = RPGTypeCrosshairsCenter;
						HUDPackage.CrosshairsLeft	= nullptr;
						HUDPackage.CrosshairsRight	= nullptr;
						HUDPackage.CrosshairsTop	= nullptr;
						HUDPackage.CrosshairsBottom = nullptr;
					}
					else if (BaseCharacter->GetEquippedWeapon()->Get_WeaponType() == EWeaponType::EWT_ScatterWeapons)
					{
						HUDPackage.CrosshairsCenter = CircleTypeCrosshairsCenter;
						HUDPackage.CrosshairsLeft	= CircleTypeCrosshairsLeft;
						HUDPackage.CrosshairsRight	= CircleTypeCrosshairsRight;
						HUDPackage.CrosshairsTop	= nullptr;
						HUDPackage.CrosshairsBottom = nullptr;
					}
					else if (BaseCharacter->GetEquippedWeapon()->Get_WeaponType() == EWeaponType::EWT_SniperRifle)
					{
						HUDPackage.CrosshairsCenter = CircleTypeCrosshairsCenter;
						HUDPackage.CrosshairsLeft	= nullptr;
						HUDPackage.CrosshairsRight	= nullptr;
						HUDPackage.CrosshairsTop	= nullptr;
						HUDPackage.CrosshairsBottom = nullptr;
					}
					else if (BaseCharacter->GetEquippedWeapon()->Get_WeaponType() == EWeaponType::EWT_GrenadeLauncher)
					{
						HUDPackage.CrosshairsCenter = CircleTypeCrosshairsCenter;
						HUDPackage.CrosshairsLeft	= CircleTypeCrosshairsLeft;
						HUDPackage.CrosshairsRight	= CircleTypeCrosshairsRight;
						HUDPackage.CrosshairsTop	= nullptr;
						HUDPackage.CrosshairsBottom = CrosshairsBottom;
					}
				}
			}
			// Crosshair Spreading
			FVector2D WalkSpeedRange(0.0f, EnhancedMovementComponent->MaxWalkSpeed / 1.5f);
			FVector2D VelocityMultiplierRange(0.0f, 1.0f);
			FVector Velocity = BaseCharacter->GetVelocity();
			Velocity.Z = 0.0f;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if (EnhancedMovementComponent->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.0f, DeltaTime, 30.0f);
			}

			if (BaseCharacter->IsAiming())
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.58f, DeltaTime, 30.f);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.0f, DeltaTime, 30.f);
			}
			// Reset CrosshairShootingFactor value at each frame. This variable getting value from Fire Function
			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.0f);

			HUDPackage.CrosshairSpread =
				0.25f +
				CrosshairVelocityFactor +
				CrosshairInAirFactor -
				CrosshairAimFactor +
				CrosshairShootingFactor;
			PlayerHUD->SetHUDPackage(HUDPackage);
		}
		else 
		{
			PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(PlayerControllerManager->GetHUD()) : PlayerHUD;
		}
	}
}

void UCameraManager::InterpFOV(float DeltaTime)
{
	if (BaseCharacter->GetEquippedWeapon() == nullptr || BaseCharacter->CharacterIsDead() || BaseCharacter->GameplayIsDisabled()) return;

	float GetZoomedFOV = BaseCharacter->GetEquippedWeapon()->GetZoomedFOV();
	float GetZoomedInterpSpeed = BaseCharacter->GetEquippedWeapon()->GetZoomInterpSpeed();
	if (BaseCharacter->IsAiming())
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, GetZoomedFOV, DeltaTime, GetZoomedInterpSpeed);
	}
	else
	{
		if (CurrentFOV != DefaultFOV)
		{
			CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, GetZoomedInterpSpeed);
		}
	}

	if (BaseCharacter && BaseCharacter->GetThirdPersonCamera())
	{
		BaseCharacter->GetThirdPersonCamera()->SetFieldOfView(CurrentFOV);
	}
}

void UCameraManager::SprintFOV(float DeltaTime)
{
	//if (BaseCharacter == nullptr || BaseCharacter->CharacterIsDead() || BaseCharacter->GameplayIsDisabled()) return;

	//float CurrentVelocitySize = BaseCharacter->GetVelocity().Size();

	//if (CurrentVelocitySize >= 400.0f)
	//{
	//	float TargetCameraLagSpeed = 1.0f;
	//	float TargetLagMaxDistance = 100.0f;

	//	BaseCharacter->GetThirdPersonSpringArm()->CameraLagSpeed		= TargetCameraLagSpeed;
	//	BaseCharacter->GetThirdPersonSpringArm()->CameraLagMaxDistance  = TargetLagMaxDistance;
	//	BaseCharacter->GetThirdPersonSpringArm()->bEnableCameraLag		= true;
	//}
	//else
	//{
	//	float TargetCameraLagSpeed = 10.0f;
	//	float TargetLagMaxDistance = ZERO;

	//	BaseCharacter->GetThirdPersonSpringArm()->CameraLagSpeed		= TargetCameraLagSpeed;
	//	BaseCharacter->GetThirdPersonSpringArm()->CameraLagMaxDistance  = TargetLagMaxDistance;

	//	if (BaseCharacter->GetThirdPersonSpringArm()->CameraLagSpeed == TargetCameraLagSpeed)
	//	{
	//		BaseCharacter->GetThirdPersonSpringArm()->bEnableCameraLag = false;
	//	}
	//}
}


void UCameraManager::Crouch_InterpolateCameraPosition(float DeltaTime)
{
	if (BaseCharacter == nullptr || BaseCharacter->GetThirdPersonCamera() == nullptr || BaseCharacter->CharacterIsDead()) return;

	if (EnhancedMovementComponent)
	{
		if (EnhancedMovementComponent->IsCrouching())
		{
			FVector TargetCameraLocation = FVector(0.f, 0.f, -60.f);
			if (BaseCharacter->GetThirdPersonCamera()->GetRelativeLocation() != TargetCameraLocation)
			{
				FVector CurrentCameraLocation = BaseCharacter->GetThirdPersonCamera()->GetRelativeLocation();
				FVector NewCameraLocation = FMath::VInterpTo(CurrentCameraLocation, TargetCameraLocation, DeltaTime, 10.0f);

				BaseCharacter->GetThirdPersonCamera()->SetRelativeLocation(NewCameraLocation);
			}
		}
		else
		{
			FVector TargetCameraLocation = FVector(0.0f, 0.0f, 0.0f);
			if (BaseCharacter->GetThirdPersonCamera()->GetRelativeLocation() != TargetCameraLocation)
			{
				FVector CurrentCameraLocation = BaseCharacter->GetThirdPersonCamera()->GetRelativeLocation();
				FVector NewCameraLocation = FMath::VInterpTo(CurrentCameraLocation, TargetCameraLocation, DeltaTime, 10.0f);

				BaseCharacter->GetThirdPersonCamera()->SetRelativeLocation(NewCameraLocation);
			}
		}
	}
}

#pragma region Camera-Shakes
void UCameraManager::PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass)
{
	if (BaseCharacter->IsLocallyControlled())
	{
		APlayerCameraManager* PlayerCameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));

		if (PlayerCameraManager)
		{
			PlayerCameraManager->StartCameraShake(CameraShakeClass, 1);
		}
	}
}
#pragma endregion