// ©2023 Reverse - A Company.All rights reserved.


#include "Character/BaseCharacter.h"

#include "Character/BaseCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CustomCameraComponent.h"
#include "Camera/CustomPlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/GameplayStatics.h"

static FName NAME_BaseCharacterCollisionProfile_Capsule (TEXT("BaseCharacterPawnCapsule"));
static FName NAME_BaseCharacterCollisionProfile_Mesh	(TEXT("BaseCharacterPawnMesh"   ));

#pragma region "Constructor"
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
/*------------------------------------------------------------------------------------------------------------------------*/
/*-----------[Base Character Class Initialization settings]---------------------------------------------------------------*/
/*---------------------------------------[Constructor]--------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
	// Network
	PrimaryActorTick.bStartWithTickEnabled = true;
	NetCullDistanceSquared = 900000000.0f;

	//Capsule|Check|Setup
	UCapsuleComponent*		CapsuleComp =
							GetCapsuleComponent();	
							check(CapsuleComp);

	CapsuleComp			->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComp			->SetCollisionProfileName(NAME_BaseCharacterCollisionProfile_Capsule);

//	#Character's appearance
	Hair				= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"  ));
	Head				= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"  ));

	Torso				= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso" ));
	Legs				= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"  ));
	Arms				= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"  ));
	Hands				= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands" ));


	Head				->SetupAttachment(CapsuleComp);

	Hair				->SetupAttachment(Head);
	Torso				->SetupAttachment(Head);
	Legs				->SetupAttachment(Head);
	Arms				->SetupAttachment(Head);
	Hands				->SetupAttachment(Head);
	/*End of appearance setup*/

//  #Camera components Initialization settings
	// Get the player controller's camera manager
	CustomCameraComponent = CreateDefaultSubobject<UCustomCameraComponent>	(TEXT("CameraComponent"	 ));

	ThirdPersonSpringArm  = CreateDefaultSubobject<USpringArmComponent>		(TEXT("TPPSpringArm"	 ));

	FirstPersonCamera	  = CreateDefaultSubobject<UCameraComponent>		(TEXT("FirstPersonCamera"));
	ThirdPersonCamera	  = CreateDefaultSubobject<UCameraComponent>		(TEXT("ViewCamera"       ));

	ThirdPersonSpringArm  ->SetupAttachment(Head);
	ThirdPersonSpringArm  ->TargetArmLength = 400.f;
	ThirdPersonSpringArm  ->bUsePawnControlRotation = true;

	ThirdPersonCamera	  ->SetupAttachment(ThirdPersonSpringArm);
	ThirdPersonCamera	  ->bUsePawnControlRotation = false;

	FirstPersonCamera	  ->SetupAttachment(Head, FName("head"));
	FirstPersonCamera	  ->bUsePawnControlRotation = true;
	FirstPersonCamera	  ->SetWorldLocation(FVector(0.0f, 20.f, 0.0f));
	FirstPersonCamera	  ->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
//	#End of Camera Setting

//	Casting Components
	BaseCharacterMovementComponent		= Cast<UBaseCharacterMovementComponent>(GetCharacterMovement());
	BaseCharacterMovementComponent->bOrientRotationToMovement = true;
	BaseCharacterMovementComponent->SetIsReplicated(true);

	bUseControllerRotationPitch	= false;
	bUseControllerRotationYaw	= false;
	bUseControllerRotationRoll	= false;
	BaseEyeHeight				= 80.0f;
	CrouchedEyeHeight			= 50.0f;

} 
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------[Base Character Class Initialization settings]--------------------------------------*/
/*----------------------------------------------------[Constructor]-------------------------------------------------------*/
/*--------------------------------------------------------[#End]----------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/

#pragma endregion "Constructor"

#pragma region "Initial func."


void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ThirdPersonCamera->SetActive(true);
	FirstPersonCamera->SetActive(false);
	CustomCameraComponent->SetCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson);

}
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* MeshComp =	GetMesh();
	if (MeshComp)
	{
		MeshComp->DestroyComponent();
		MeshComp->SetActive(false);
		MeshComp = nullptr;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

 

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BaseCharacterMovementComponent->IsProneButtonActive();
}
	
#pragma endregion "Game Loop Functions"

#pragma region Input System General
#pragma region InputSetups
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(CameraSwitchAction,	ETriggerEvent::Started,   this, &ABaseCharacter::SwitchCameraInput);

		EnhancedInputComponent->BindAction(MoveInputAction,		ETriggerEvent::Triggered, this, &ABaseCharacter::MovementInput_Activate);
		EnhancedInputComponent->BindAction(MoveInputAction,		ETriggerEvent::Completed, this, &ABaseCharacter::MovementInput_Deactivate);
		EnhancedInputComponent->BindAction(LookInputAction,		ETriggerEvent::Triggered, this, &ABaseCharacter::LookInput);
		EnhancedInputComponent->BindAction(JumpInputAction,		ETriggerEvent::Triggered, this, &ABaseCharacter::Jump);
		EnhancedInputComponent->BindAction(SprintInputAction,	ETriggerEvent::Triggered, this, &ABaseCharacter::SprintInput_Activate);
		EnhancedInputComponent->BindAction(SprintInputAction,	ETriggerEvent::Completed, this, &ABaseCharacter::SprintInput_Deactivate);
		EnhancedInputComponent->BindAction(CrouchInputAction,	ETriggerEvent::Triggered, this, &ABaseCharacter::CrouchInput_Activate);
		EnhancedInputComponent->BindAction(CrouchInputAction,	ETriggerEvent::Completed, this, &ABaseCharacter::CrouchInput_Deactivate);
		EnhancedInputComponent->BindAction(SlideInputAction,	ETriggerEvent::Started,   this, &ABaseCharacter::SlideInput_Activate);
		EnhancedInputComponent->BindAction(SlideInputAction,	ETriggerEvent::Completed, this, &ABaseCharacter::SlideInput_Deactivate);
		EnhancedInputComponent->BindAction(ProneInputAction,	ETriggerEvent::Started,   this, &ABaseCharacter::ProneInput_Toggle);

		EnhancedInputComponent->BindAction(DashInputAction,		ETriggerEvent::Started,   this, &ABaseCharacter::DashInput_Activate);
		EnhancedInputComponent->BindAction(DashInputAction,		ETriggerEvent::Completed, this, &ABaseCharacter::DashInput_Deactivate);


		
	}
}

void ABaseCharacter::Jump()
{
	Super::Jump();
	bCustomJumpPressed = true;
	bPressedJump = false;
}
void ABaseCharacter::StopJumping()
{
	Super::StopJumping();

	bCustomJumpPressed = false;
}

#pragma endregion

#pragma region Camera Setup
void ABaseCharacter::SwitchCameraInput(const FInputActionValue& Value)
{
	if (ThirdPersonCamera->IsActive())
	{
		FirstPersonPerspective();
	}
	else
	{
		ThirdPersonPerspective();
	}
}


void ABaseCharacter::CalculateAndClampAimOffset()
{
	FRotator AimRotation = this->GetBaseAimRotation();
	FRotator CurrentCharacterRotation = this->GetActorRotation();
	FRotator AimOffset = AimRotation - CurrentCharacterRotation;

	AimOffset.Normalize();

	AimOffSet_Pitch = AimOffset.Pitch;
	AimOffSet_Yaw	= AimOffset.Yaw;

	AimOffSet_Yaw	= FMath::ClampAngle(AimOffSet_Yaw, -90.0f, 90.0f);
	AimOffSet_Pitch = FMath::ClampAngle(AimOffSet_Pitch, -90.0f, 90.0f);
}


void ABaseCharacter::ThirdPersonPerspective()
{
	CustomCameraComponent->SetCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson);
	ThirdPersonCamera	 ->SetActive(true);
	FirstPersonCamera	 ->SetActive(false);
	Hair				 ->SetVisibility(true);
	Head				 ->SetVisibility(true);
}

void ABaseCharacter::FirstPersonPerspective()
{
	CustomCameraComponent	->SetCameraViewPerspective(ECameraViewPerspective::ECVP_FirstPerson);
	ThirdPersonCamera		->SetActive(false);
	FirstPersonCamera		->SetActive(true);

	if (IsLocallyControlled())
	{
		Hair	->SetVisibility(false);
		Head	->SetVisibility(false);
	}
	else
	{
		Hair	->SetVisibility(true);
		Head	->SetVisibility(true);
	}
}
#pragma endregion

#pragma region Move&&LookInput


void ABaseCharacter::MovementInput_Activate(const FInputActionValue& Value)
{
	if (!bIsMovementInputActive) { bIsMovementInputActive = true; }

	if (CustomCameraComponent->IsCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson))
	{
		if (Controller != NULL)
		{
			const FVector2D MovementVector = Value.Get<FVector2D>();
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			if (BaseCharacterMovementComponent->IsSliding())
			{
				ForwardDirection = FVector::ZeroVector;
			}

			const FVector MovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);
			AddMovementInput(MovementDirection);
		}
	}

	if (CustomCameraComponent->IsCameraViewPerspective(ECameraViewPerspective::ECVP_FirstPerson))
	{
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != NULL)
		{
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);


			//Horizontal
			if (MovementVector.X >= 1.0f)
			{
				isMovingRight = true;
				isMovingLeft  = false;
			}
			else if (MovementVector.X <= -1.0f)
			{
				isMovingLeft  = true;
				isMovingRight = false;
			}


			// Vertical
			if (MovementVector.Y >= 1.0f)
			{
				isMovingForward  = true;
				isMovingBackward = false;
			}
			else if (MovementVector.Y <= -1.0f)
			{
				isMovingBackward = true;
				isMovingForward  = false;
			}

			HorizontalDirectionsValue_FPP	= MovementVector.X;
			VerticalDirectionsValue_FPP		= MovementVector.Y;
		}
	}
}

void ABaseCharacter::MovementInput_Deactivate(const FInputActionValue& Value)
{
	if (bIsMovementInputActive) {bIsMovementInputActive = false;}

	if (CustomCameraComponent->IsCameraViewPerspective(ECameraViewPerspective::ECVP_FirstPerson))
	{
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != NULL)
		{
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);
			if ( MovementVector.X == 0.0f )
			{
					isMovingRight = false;
					isMovingLeft = false;
			}
				
			if (MovementVector.Y == 0.0f)
			{
				isMovingForward  = false;
				isMovingBackward = false;
			}
		}
		HorizontalDirectionsValue_FPP	= MovementVector.X;
		VerticalDirectionsValue_FPP		= MovementVector.Y;
	}
}


void ABaseCharacter::LookInput(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	float LookingSensitivityviaMouse = CustomCameraComponent->GetMouseLookSensitivity();

	float ScaledPitchInput	= LookAxisVector.Y * -1.0f;
	float ScaledYawInput	= LookAxisVector.X * LookingSensitivityviaMouse;

	AddControllerPitchInput(ScaledPitchInput);
	AddControllerYawInput  (ScaledYawInput);
}
#pragma endregion


#pragma region SprintInput
void ABaseCharacter::SprintInput_Activate(const FInputActionValue& Value)
{
	if (BaseCharacterMovementComponent->IsMovingOnGround())
	{
		LOG_WARNING("IsMovingOnGround");
	}
	else
	{
		LOG_WARNING("NotMovingOnGround")
	}





	if (BaseCharacterMovementComponent->CanSprint())
	{
		LOG_WARNING("Inside");
		BaseCharacterMovementComponent->SetSprintInput(true);
	}
}

void ABaseCharacter::SprintInput_Deactivate(const FInputActionValue& Value)
{
	if (BaseCharacterMovementComponent->IsSprinting())
	{
		BaseCharacterMovementComponent->SetSprintInput(false);
	}
}
#pragma endregion
#pragma region CrouchInput
void ABaseCharacter::CrouchInput_Activate(const FInputActionValue& Value)
{
	if (BaseCharacterMovementComponent->CanCrouch())
	{
		BaseCharacterMovementComponent->SetCrouchInput(true);
	}
}

void ABaseCharacter::CrouchInput_Deactivate(const FInputActionValue& Value)
{
	if (BaseCharacterMovementComponent->IsCrouchingCustom())
	{
		BaseCharacterMovementComponent->SetCrouchInput(false);
	}
}

#pragma endregion
#pragma region JumpInput
void ABaseCharacter::JumpInput(const FInputActionValue& Value)
{
	if ( BaseCharacterMovementComponent-> IsFalling()      ||
		 BaseCharacterMovementComponent-> IsCrouching()    || 
		 BaseCharacterMovementComponent-> IsCrouchingCustom()
		)
	{
		return;
	}

	Super::Jump();
}
#pragma endregion
#pragma region AimInput
void ABaseCharacter::AimButtonInput_Activate(const FInputActionValue& Value)
{

}

void ABaseCharacter::AimButtonInput_Deactivate(const FInputActionValue& Value)
{

}
#pragma endregion

#pragma region SlideInput

void ABaseCharacter::SlideInput_Activate(const FInputActionValue& Value)
{
	// Activates sliding input if sliding is allowed.
	if (BaseCharacterMovementComponent->ShouldAllowSlideButtonPress())
	{
		if (BaseCharacterMovementComponent->CanSlide())
		{
			if (!BaseCharacterMovementComponent->GetSafeWantsToSlide())
			{
				BaseCharacterMovementComponent->SetSlideInput(true);
			}
		}
	}
}

void ABaseCharacter::SlideInput_Deactivate(const FInputActionValue& Value)
{
	// Deactivates sliding input if the character is currently sliding.
	if (BaseCharacterMovementComponent->IsSliding())
	{
		BaseCharacterMovementComponent->SetSlideInput(false);
	}
}


#pragma endregion
#pragma region |Dash|
void ABaseCharacter::DashInput_Activate(const FInputActionValue& Value)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	float DashStartTime			= BaseCharacterMovementComponent->DashStartTime;
	float DashCooldownDuration  = BaseCharacterMovementComponent->DashCooldownDuration;

	if (CurrentTime - DashStartTime >= DashCooldownDuration)
	{
		BaseCharacterMovementComponent->SetDashInput(true);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(BaseCharacterMovementComponent->DashCooldown_Timerhandle,
		BaseCharacterMovementComponent,
		&UBaseCharacterMovementComponent::OnDashCooldownFinished,
		DashCooldownDuration - (CurrentTime - DashStartTime));
	}
}

void ABaseCharacter::DashInput_Deactivate(const FInputActionValue& Value)
{
	GetWorld()->GetTimerManager().ClearTimer(BaseCharacterMovementComponent->DashCooldown_Timerhandle);
	BaseCharacterMovementComponent->SetDashInput(false);
}
#pragma endregion

#pragma region |Prone|
void ABaseCharacter::ProneInput_Toggle(const FInputActionValue& Value)
{
	UBaseCharacterMovementComponent* BCMC = BaseCharacterMovementComponent;

	if (BCMC->IsProneButtonActive())
	{
		LOG_WARNING("TRUE");
	}
	else
	{
		LOG_WARNING("FALSE");
	}


	if (BCMC->IsProneButtonActive() == false)
	{
		if (BCMC->CanProne())
		{
			BCMC->SetProneInput(!BCMC->ControllerWantsToProne());

			if (!BCMC->ControllerWantsToProne())
			{
				BCMC->SetProneInput(false);
			}
			BCMC->SetProneInputCooldown(true);
		}
	}
}

#pragma endregion

#pragma region Getter Functionalities


FCollisionQueryParams ABaseCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}
#pragma endregion
#pragma region Setter Functionalities
#pragma endregion