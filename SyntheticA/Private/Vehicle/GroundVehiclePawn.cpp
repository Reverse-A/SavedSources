// @2023 All rights reversed by Reverse-Alpha Studios


#include "Vehicle/GroundVehiclePawn.h"
#include "Vehicle/Wheel/VehicleWheelFront.h"
#include "Vehicle/Wheel/VehicleWheelRear.h"
#include "ChaosWheeledVehicleMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Characters/BaseCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerManager.h"
#include "Sound/SoundCue.h"
#include "InputActionValue.h"

AGroundVehiclePawn::AGroundVehiclePawn()
{
/*FrontCamera*/
	FrontSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Front Spring Arm"));
	FrontSpringArm->SetupAttachment(GetMesh());
	FrontSpringArm->TargetArmLength				= 0.0f;
	FrontSpringArm->bDoCollisionTest			= false;
	FrontSpringArm->bEnableCameraRotationLag	= true;
	FrontSpringArm->CameraRotationLagSpeed		= 15.0f;
	FrontSpringArm->SetRelativeLocation(FVector(30.0f, 0.0f, 120.0f));

	FrontCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Front Camera"));
	FrontCamera->SetupAttachment(FrontSpringArm);
	FrontCamera->bAutoActivate					= false;

/*Behind Camera*/
	BehindSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Behind Spring Arm"));
	BehindSpringArm->SetupAttachment(GetMesh());
	BehindSpringArm->TargetArmLength			= 650.0f;
	BehindSpringArm->SocketOffset.Z				= 150.0f;
	BehindSpringArm->bDoCollisionTest			= false;
	BehindSpringArm->bInheritPitch				= false;
	BehindSpringArm->bInheritRoll				= false;
	BehindSpringArm->bEnableCameraRotationLag	= true;
	BehindSpringArm->CameraRotationLagSpeed		= 2.0f;
	BehindSpringArm->CameraLagMaxDistance		= 50.0f;

	BehindCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Behind Camera"));
	BehindCamera->SetupAttachment(BehindSpringArm);

	ChaosVehicleMovement = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

}

void AGroundVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// steering 
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AGroundVehiclePawn::Steering);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AGroundVehiclePawn::Steering);

		// throttle 
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AGroundVehiclePawn::Throttle);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AGroundVehiclePawn::Throttle);

		// break 
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AGroundVehiclePawn::Brake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Started, this, &AGroundVehiclePawn::StartBrake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AGroundVehiclePawn::StopBrake);

		// handbrake 
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &AGroundVehiclePawn::StartHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &AGroundVehiclePawn::StopHandbrake);

		// look around 
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &AGroundVehiclePawn::LookAround);

		// camera 
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &AGroundVehiclePawn::InputAction_ToggleCamera);
		EnhancedInputComponent->BindAction(CameraZoomInOutAction, ETriggerEvent::Started, this, &AGroundVehiclePawn::InputAxis_ZoomInOut);

		// reset the vehicle 
		EnhancedInputComponent->BindAction(ResetVehicleAction,	ETriggerEvent::Started, this, &AGroundVehiclePawn::ResetVehicle);

		// Exit the vehicle
		EnhancedInputComponent->BindAction(ExitGroundVehicleInputAction, ETriggerEvent::Started, this, &AGroundVehiclePawn::ExitVehicle);

		// Enable or disable radioplayer
		EnhancedInputComponent->BindAction(PlayRadioInputAction, ETriggerEvent::Started, this, &AGroundVehiclePawn::InputAction_PlayRadio);

	}
}

void AGroundVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputGroundVehicleMappingContext, 0);
		}
	}

	/*Overlap-Setup*/
	GetOverlapCollision()->OnComponentBeginOverlap.AddDynamic(this, &AGroundVehiclePawn::OnBoxCollisionOverlap);
	GetOverlapCollision()->OnComponentEndOverlap.AddDynamic(this, &AGroundVehiclePawn::OnBoxCollisionEndOverlap);
}

void AGroundVehiclePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	bool bMovingOnGround = ChaosVehicleMovement->IsMovingOnGround();
	GetMesh()->SetAngularDamping(bMovingOnGround ? 0.0f : 3.0f);

	float CameraYaw = BehindSpringArm->GetRelativeRotation().Yaw;
	CameraYaw = FMath::FInterpTo(CameraYaw, 0.0f, DeltaSeconds, 1.0f);

	BehindSpringArm->SetRelativeRotation(FRotator(0.0f, CameraYaw, 1.0f));

	if (ChaosVehicleMovement && !ChaosVehicleMovement->UpdatedPrimitive->IsAnyRigidBodyAwake())
	{
		ChaosVehicleMovement->UpdatedPrimitive->WakeAllRigidBodies();	
	}

	SmoothZoomInOut(bInterpolateZoom);
}
#pragma region Overlapping
void AGroundVehiclePawn::OnBoxCollisionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BaseCharacter = Cast<ABaseCharacter>(OtherActor);

	if (BaseCharacter)
	{
		BaseCharacter->SetGroundVehicleClass(this);
	}
}

void AGroundVehiclePawn::OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (BaseCharacter)
	{
		BaseCharacter->SetGroundVehicleClass(nullptr);
	}
}

#pragma endregion
void AGroundVehiclePawn::Steering(const FInputActionValue& Value)
{
	float SteeringValue = Value.Get<float>();

	ChaosVehicleMovement->SetSteeringInput(SteeringValue);
}

void AGroundVehiclePawn::Throttle(const FInputActionValue& Value)
{
	float ThrottleValue = Value.Get<float>();

	ChaosVehicleMovement->SetThrottleInput(ThrottleValue);
}


void AGroundVehiclePawn::Brake(const FInputActionValue& Value)
{
	float BreakValue = Value.Get<float>();

	ChaosVehicleMovement->SetBrakeInput(BreakValue);

}

void AGroundVehiclePawn::StartBrake(const FInputActionValue& Value)
{
	BrakeLights(true);
}

void AGroundVehiclePawn::StopBrake(const FInputActionValue& Value)
{
	BrakeLights(false);

	ChaosVehicleMovement->SetBrakeInput(0.0f);
}

void AGroundVehiclePawn::StartHandbrake(const FInputActionValue& Value)
{
	ChaosVehicleMovement->SetHandbrakeInput(true);
	BrakeLights(true);
}

void AGroundVehiclePawn::StopHandbrake(const FInputActionValue& Value)
{
	ChaosVehicleMovement->SetHandbrakeInput(false);
	BrakeLights(false);
}

void AGroundVehiclePawn::LookAround(const FInputActionValue& Value)
{
	float LookValue = Value.Get<float>();
	BehindSpringArm->AddLocalRotation(FRotator(0.0f, LookValue, 0.0f));
}

void AGroundVehiclePawn::ResetVehicle(const FInputActionValue& Value)
{
	FVector ResetLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);

	FRotator ResetRotation = GetActorRotation();
	ResetRotation.Pitch = 0.0f;
	ResetRotation.Roll = 0.0f;

	SetActorTransform(FTransform(ResetRotation, ResetLocation, FVector::OneVector), false, nullptr, ETeleportType::TeleportPhysics);

	GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);

}

/*Camera*/
void AGroundVehiclePawn::InputAction_ToggleCamera(const FInputActionValue& Value)
{
	bFrontCameraActive = !bFrontCameraActive;

	FrontCamera->SetActive(bFrontCameraActive);
	BehindCamera->SetActive(!bFrontCameraActive);
}

void AGroundVehiclePawn::InputAxis_ZoomInOut(const FInputActionValue& Value)
{
	if (FrontCamera->IsActive() && bInterpolateZoom == true) return;

	float ZoomInputValue				= Value.Get<float>();
	const float MinZoom					= 650.0f;				
	const float MaxZoom					= 1000.0f;

	float CurrentArmLength				= BehindSpringArm->TargetArmLength;
	NewArmLength						= FMath::Clamp(CurrentArmLength - ZoomInputValue * 50.0f, MinZoom, MaxZoom);

	bInterpolateZoom = true;
}

void AGroundVehiclePawn::SmoothZoomInOut(bool bSmooth)
{
	if (bInterpolateZoom)
	{
		float DeltaSeconds						= GetWorld()->GetDeltaSeconds();
		const float ZoomInterpolationSpeed		= 20.0f;
		const float MultiplierSeconds			= 5.0f;
		float CurrentArmLength					= BehindSpringArm->TargetArmLength;

		float LerpedArmLength =  /*Warning: LerpingBelow*/
		FMath::FInterpConstantTo
		(
			CurrentArmLength,
			NewArmLength,
			DeltaSeconds * MultiplierSeconds,
			ZoomInterpolationSpeed
		);

		BehindSpringArm->TargetArmLength = LerpedArmLength;

		const float MinZoom = 650.0f;
		const float MaxZoom = 1000.0f;
		float Proportion = (NewArmLength - MinZoom) / (MaxZoom - MinZoom);
		float NewSocketOffZ = FMath::Lerp(150.0f, 250.0f, Proportion);

		FVector CurrentSocketOffSet = BehindSpringArm->SocketOffset;
		FVector TargetSocketOffSet = FVector(0.0f, 0.0f, NewSocketOffZ);
		FVector LerpedSocketOffSet = FMath::VInterpConstantTo
		(
			CurrentSocketOffSet,
			TargetSocketOffSet,
			DeltaSeconds * MultiplierSeconds,
			ZoomInterpolationSpeed
		);

		BehindSpringArm->SocketOffset = LerpedSocketOffSet;

		if (FMath::IsNearlyEqual(LerpedArmLength, NewArmLength, 0.01f)) // Adjust tolerance as needed
		{
			bInterpolateZoom = false;
		}
	}
}

void AGroundVehiclePawn::BrakeLights(bool bBraking)
{

}

void AGroundVehiclePawn::ExitVehicle()
{
	if (bPlayerEnteredVehicle == false || BaseCharacter == nullptr || GetExitArrow() == nullptr) return;

	if (BaseCharacter)
	{
		/*AdjustCharacter First*/
		BaseCharacter->SetActorTransform(GetExitArrow()->GetComponentTransform(), false, nullptr, ETeleportType::ResetPhysics);
		BaseCharacter->SetPlayerInVehicle(false);
		SetPlayerEnteredVehicle(false);

		APlayerControllerManager* PlayerControllerManager = Cast<APlayerControllerManager>(Controller);
		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDIfPlayerInVehicle(false);
		}

		APlayerController* PlayerController = Cast<APlayerController>(Controller);

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			FModifyContextOptions ModifyContextOption;
			ModifyContextOption.bForceImmediately;
			Subsystem->RemoveMappingContext(InputGroundVehicleMappingContext, ModifyContextOption);
			Subsystem->AddMappingContext(BaseCharacter->GetBaseCharacterMappingContext(), 1);
		}

		if (PlayerController)
		{
			PlayerController->Possess(BaseCharacter);
		}

		float CurrentForwardSpeed = GetChaosVehicleMovement()->GetForwardSpeed();
		
		if (CurrentForwardSpeed > 800)
		{
			LOG_WARNING("TODO: Reduce the vehicle's speed after exiting the vehicle")
		}



		bPlayerEnteredVehicle = false;
	}
}

void AGroundVehiclePawn::InputAction_PlayRadio()
{
	bRadioStarted = !bRadioStarted;

	if (bRadioStarted)
	{
		PlayRadio();
	}
	else
	{
		RadioAudioComponent->Stop();
	}
}



#pragma region Radio
void AGroundVehiclePawn::PlayRadio()
{
	if (RadioPlaylist)
	{
		RadioAudioComponent = UGameplayStatics::SpawnSoundAttached
		(
			RadioPlaylist,
			RootComponent,
			NAME_None,
			FVector::ZeroVector,
			EAttachLocation::KeepWorldPosition,
			false,
			1.0f,
			1.0f,
			0.0f
		);
	}
}
#pragma endregion