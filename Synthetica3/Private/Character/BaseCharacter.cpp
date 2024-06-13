// ©2023 Reverse - A Company. All rights reserved.

#include "Character/BaseCharacter.h"

#include "Animation/CoreAnimProcessor.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraSettings.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "Components/EnhancedMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/CoreGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerController/PlayerControllerManager.h"
#include "PlayerState/PlayerStateManager.h"
#include "Sound/SoundCue.h"

#include "Weapons/Weapon.h"
#include "Net/UnrealNetwork.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"



#pragma region | Constructor |
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UEnhancedMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bStartWithTickEnabled = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	bNetLoadOnClient		= true;
	bNetUseOwnerRelevancy	= false;
	NetDormancy				= DORM_Awake;
	NetCullDistanceSquared	= 900000000.0f;
	NetUpdateFrequency		= 66.0f;
	MinNetUpdateFrequency	= 33.0f;
	NetPriority				= 3.0f;
		

	UCapsuleComponent* CapsuleComp	= GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp						->InitCapsuleSize(35.0f, 89.95f);
	CapsuleComp						->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()						->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Hair							= CreateDefaultSubobject<USkeletalMeshComponent>		(TEXT("Hair" ));
	Head							= CreateDefaultSubobject<USkeletalMeshComponent>		(TEXT("Head" ));

	Torso							= CreateDefaultSubobject<USkeletalMeshComponent>		(TEXT("Torso"));
	Legs							= CreateDefaultSubobject<USkeletalMeshComponent>		(TEXT("Legs" ));
	Arms							= CreateDefaultSubobject<USkeletalMeshComponent>		(TEXT("Arms" ));
	Hands							= CreateDefaultSubobject<USkeletalMeshComponent>		(TEXT("Hands"));
	Bag								= CreateDefaultSubobject<USkeletalMeshComponent>		(TEXT("Bag"	 ));

	Hair							->SetCollisionObjectType(ECC_SkeletalMesh);
	Head							->SetCollisionObjectType(ECC_SkeletalMesh);
	Torso							->SetCollisionObjectType(ECC_SkeletalMesh);
	Legs							->SetCollisionObjectType(ECC_SkeletalMesh);
	Arms							->SetCollisionObjectType(ECC_SkeletalMesh);
	Hands							->SetCollisionObjectType(ECC_SkeletalMesh);
	Bag								->SetCollisionObjectType(ECC_SkeletalMesh);


	Hair							->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Head							->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Torso							->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Legs							->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Arms							->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Hands							->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Bag								->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	
	Head							->SetupAttachment(CapsuleComp);

	Hair							->SetupAttachment(Head);
	Torso							->SetupAttachment(Head);
	Legs							->SetupAttachment(Head);
	Arms							->SetupAttachment(Head);
	Hands							->SetupAttachment(Head);
	Bag								->SetupAttachment(Head);

	/* Camera Properties */
	ThirdPersonSpringArm			= CreateDefaultSubobject<USpringArmComponent>			(TEXT("TPPSpringArm"			  ));
	CameraSettings					= CreateDefaultSubobject<UCameraSettings>				(TEXT("CameraComponent"			  ));
	FirstPersonCamera				= CreateDefaultSubobject<UCameraComponent>				(TEXT("FirstPersonCamera"		  ));
	ThirdPersonCamera				= CreateDefaultSubobject<UCameraComponent>				(TEXT("ViewCamera"				  ));

	ThirdPersonSpringArm			->SetupAttachment(Head);
	ThirdPersonSpringArm			->TargetArmLength = 400.f;
	ThirdPersonSpringArm			->bUsePawnControlRotation = true;

	ThirdPersonCamera				->SetupAttachment(ThirdPersonSpringArm);
	ThirdPersonCamera				->bUsePawnControlRotation = false;

	FirstPersonCamera				->SetupAttachment (Head, FName("head"));
	FirstPersonCamera				->bUsePawnControlRotation = true;
	FirstPersonCamera				->SetWorldLocation(FVector (0.000f, 20.0f, 0.00f));
	FirstPersonCamera				->SetWorldRotation(FRotator(-90.0f, 0.00f, 90.0f));
	FirstPersonCamera				->bAutoActivate	= false;

	EnhancedMovement				= Cast<UEnhancedMovementComponent>(GetCharacterMovement());
	EnhancedMovement				->bOrientRotationToMovement	= true;
	EnhancedMovement				->SetIsReplicated(true);

	Combat							= CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat							->SetIsReplicated(true);
	bUseControllerRotationPitch		= false;
	bUseControllerRotationYaw		= false;
	bUseControllerRotationRoll		= false;
	BaseEyeHeight					= 80.0f;
	CrouchedEyeHeight				= 50.0f;

	DissolveTimeline				= CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));
}
#pragma endregion | Constructor |


void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABaseCharacter, OverlappingWeapon, COND_OwnerOnly);

	DOREPLIFETIME(ABaseCharacter, Head						);
	DOREPLIFETIME(ABaseCharacter, CrouchTransformLocation	);
	DOREPLIFETIME(ABaseCharacter, MovementDirectionY		);
	DOREPLIFETIME(ABaseCharacter, MovementDirectionX		);
	DOREPLIFETIME(ABaseCharacter, MovementInputActivated	);

	DOREPLIFETIME(ABaseCharacter, Health);
	DOREPLIFETIME(ABaseCharacter, bDisableGameplay);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->BaseCharacter = this;
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	EnhancedMovement		= EnhancedMovement			== nullptr	? Cast<UEnhancedMovementComponent>	(GetCharacterMovement())  : EnhancedMovement;
	CameraSettings			= CameraSettings			== nullptr	? Cast<UCameraSettings>				(CameraSettings)		  : CameraSettings;
	CoreAnimProcessor		= CoreAnimProcessor			== nullptr	? Cast<UCoreAnimProcessor>			(Head->GetAnimInstance()) : CoreAnimProcessor;

	UpdateHUDHealth();
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABaseCharacter::ReceiveDamage);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PollInitialize();
}

#pragma region | Input |
void ABaseCharacter::CastPlayerInputComponent()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(CameraSwitchAction,		 ETriggerEvent::Started,		this, &ABaseCharacter::SwitchCamera_Input);

		EnhancedInputComponent->BindAction(MoveInputAction,			 ETriggerEvent::Triggered,		this, &ABaseCharacter::Movement_Input);
		EnhancedInputComponent->BindAction(MoveInputAction,			 ETriggerEvent::Completed,		this, &ABaseCharacter::Movement_Input_Deactivation);
		EnhancedInputComponent->BindAction(JumpInputAction,			 ETriggerEvent::Triggered,		this, &ABaseCharacter::Jump);

		EnhancedInputComponent->BindAction(LookInputAction,			 ETriggerEvent::Triggered,		this, &ABaseCharacter::Look_Input);
		EnhancedInputComponent->BindAction(InteractionAction,		 ETriggerEvent::Started,		this, &ABaseCharacter::Interaction_Input);

		EnhancedInputComponent->BindAction(AimInputAction,			 ETriggerEvent::Started,		this, &ABaseCharacter::AimButtonInput_Activation);
		EnhancedInputComponent->BindAction(AimInputAction,			 ETriggerEvent::Completed,		this, &ABaseCharacter::AimButtonInput_Deactivation);

		EnhancedInputComponent->BindAction(ReloadInputAction,		 ETriggerEvent::Started,		this, &ABaseCharacter::ReloadButtonInput);

		EnhancedInputComponent->BindAction(FireTriggerInputAction,	 ETriggerEvent::Triggered,		this, &ABaseCharacter::FireButtonInput_Activation);
		EnhancedInputComponent->BindAction(FireTriggerInputAction,	 ETriggerEvent::Completed,		this, &ABaseCharacter::FireButtonInput_Deactivation);

		EnhancedInputComponent->BindAction(SprintInputAction,		 ETriggerEvent::Triggered,		this, &ABaseCharacter::Sprint_InputActivation);
		EnhancedInputComponent->BindAction(SprintInputAction,		 ETriggerEvent::Completed,		this, &ABaseCharacter::Sprint_InputDeactivation);

		EnhancedInputComponent->BindAction(CrouchInputAction,		 ETriggerEvent::Triggered,		this, &ABaseCharacter::CrouchInput_Activation);
		EnhancedInputComponent->BindAction(CrouchInputAction,		 ETriggerEvent::Completed,		this, &ABaseCharacter::CrouchInput_Deactivation);

		EnhancedInputComponent->BindAction(ProneInputAction,		 ETriggerEvent::Started,		this, &ABaseCharacter::ProneInput);
		EnhancedInputComponent->BindAction(ProneInputAction,		 ETriggerEvent::Completed,		this, &ABaseCharacter::ProneInputReleased);
		
		EnhancedInputComponent->BindAction(SlideInputAction,		 ETriggerEvent::Started,		this, &ABaseCharacter::SlideInput_Activation);
		EnhancedInputComponent->BindAction(SlideInputAction,		 ETriggerEvent::Completed,		this, &ABaseCharacter::SlideInput_Deactivation);

		EnhancedInputComponent->BindAction(AutoRunToggleInputAction, ETriggerEvent::Started,		this, &ABaseCharacter::AutoRun_Toggle);
	}
}

void ABaseCharacter::SwitchCamera_Input(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	CameraSettings->SwitchCamera();
}

#pragma region MovementInput
void ABaseCharacter::Movement_Input(const FInputActionValue& Value)
{
	if (EnhancedMovement->IsMantling() || GameplayIsDisabled())
	{
		return;
	}

	if (HasAuthority())
	{
		MovementInputActivated = true;
	}
	else
	{
		Server_SetMovementInput(true);
	}

	if (CanAutoRun)
	{
		if (MovementDirectionY != 0 || MovementDirectionX != 0)
		{
			if (CanAutoRun)
			{
				CanAutoRun = false;
			}
			if (bIsAutoRunEnabled)
			{
				bIsAutoRunEnabled = false;
			}
		}
	}

	if (CameraSettings->IsCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson))
	{
		if (Controller != NULL)
		{
			if (IsAiming())
			{
				MovementVector = Value.Get<FVector2D>();
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				const FVector RightDirection	= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				const FVector ForwardDirection	= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

				const FVector MovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);
				AddMovementInput(MovementDirection);
				if (HasAuthority())
				{
					MovementDirectionX = MovementVector.X;
					MovementDirectionY = MovementVector.Y;
				}
				else
				{
					MovementDirectionX = MovementVector.X;
					MovementDirectionY = MovementVector.Y;
					Server_MovementDirectionValues(MovementVector.X, MovementVector.Y);
				}	
			}
			else
			{
				MovementVector = Value.Get<FVector2D>();
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

				const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				if (EnhancedMovement->IsSliding())
				{
					ForwardDirection = FVector::ZeroVector;
				}

				FVector MovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);

				AddMovementInput(MovementDirection);

				if (HasAuthority())
				{
					MovementDirectionX = MovementVector.X;
					MovementDirectionY = MovementVector.Y;
				}
				else
				{
					Server_MovementDirectionValues(MovementDirectionX, MovementDirectionY);
				}
			}
		}
	}
	else if (CameraSettings->IsCameraViewPerspective(ECameraViewPerspective::ECVP_FirstPerson))
	{
		if (Controller != NULL)
		{
			MovementVector = Value.Get<FVector2D>();

			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);
			if (HasAuthority())
			{
				MovementDirectionX = MovementVector.X;
				MovementDirectionY = MovementVector.Y;
			}
			else
			{
				MovementDirectionX = MovementVector.X;
				MovementDirectionY = MovementVector.Y;
				Server_MovementDirectionValues(MovementDirectionX, MovementDirectionY);
			}
		}
	}
}

void ABaseCharacter::Movement_Input_Deactivation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (IsLocallyControlled())
	{
		Server_SetMovementInput(false);
	}
}


void ABaseCharacter::Server_SetMovementInput_Implementation(bool IsActive)
{
	MovementInputActivated = IsActive;
}

void ABaseCharacter::Server_MovementDirectionValues_Implementation(float MovementVectorX, float MovementVectorY)
{
	Multicast_MovementDirectionsValues(MovementVectorX, MovementVectorY);
}

void ABaseCharacter::Multicast_MovementDirectionsValues_Implementation(float MovementVectorX, float MovementVectorY)
{
	MovementDirectionX = MovementVectorX;
	MovementDirectionY = MovementVectorY;
}

#pragma endregion

void ABaseCharacter::Look_Input(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	float LookingSensitivityviaMouse = CameraSettings->GetMouseLookSensitivityValue();

	float ScaledPitchInput			 = LookAxisVector.Y * -1.0f * LookingSensitivityviaMouse;
	float ScaledYawInput			 = LookAxisVector.X * LookingSensitivityviaMouse;

	AddControllerPitchInput	(ScaledPitchInput);
	AddControllerYawInput	(ScaledYawInput);
}

#pragma region |Weapon Types|

void ABaseCharacter::AimButtonInput_Activation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (Combat->EquippedWeapon == nullptr) return;

	if (Combat && !IsAiming())
	{
		if (bCached_Aiming != true)
		{
			bCached_Aiming = true;
		}
		Combat->SetAiming(true);
	}
}

void ABaseCharacter::AimButtonInput_Deactivation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (Combat->IsFireButtonPressed())
	{
		bCached_Aiming = false;
		return;
	}
	if (Combat && IsAiming())
	{
		if (bCached_Aiming != false)
		{
			bCached_Aiming = false;
		}	
		Combat->SetAiming(false);
	}
}

void ABaseCharacter::FireButtonInput_Activation(const FInputActionValue& Value)
{

	if ((Combat && Combat->EquippedWeapon == nullptr) || GameplayIsDisabled()) return;

	if (!IsAiming())
	{
		if (bCanFireIfDelayPassed != true)
		{
			float DeltaTime = GetWorld()->GetDeltaSeconds();
			bCanFireIfDelayPassTimer += DeltaTime;

			if (bCanFireIfDelayPassTimer > 0.3f)
			{
				bCanFireIfDelayPassed = true;
			}
		}
	}
	else
	{
		bCanFireIfDelayPassed = true;
	}

	if (bCanFireIfDelayPassed)
	{
		if (IsAiming())
		{
			Combat->SetFireButtonTrigger(true);
		}
		else
		{
			if (Combat->bAiming != true)
			{
				Combat->bAiming = true;
			}
			Combat->SetFireButtonTrigger(true);
		}
	}
}

void ABaseCharacter::FireButtonInput_Deactivation(const FInputActionValue& Value)
{
	if ((Combat && Combat->EquippedWeapon == nullptr) || GameplayIsDisabled()) return;

	if (bCached_Aiming)
	{
		Combat->SetAiming(true);
		bCached_Aiming = true;
	}
	else
	{
		Combat->SetAiming(false);
		bCached_Aiming = false;
	}
	Combat->SetFireButtonTrigger(false);
	bCanFireIfDelayPassed = false;
	bCanFireIfDelayPassTimer = 0.0f;
}

void ABaseCharacter::ReloadButtonInput(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (Combat)
	{
		Combat->Reload();
	}
}

bool ABaseCharacter::IsAiming()
{
	return (Combat && Combat->bAiming);
}
#pragma endregion


void ABaseCharacter::Interaction_Input(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (OverlappingWeapon == nullptr)
	{
		return;
	}
	else
	{
		if (OverlappingWeapon->IsWeaponState(EWeaponState::EWS_Equipped))
		{
			return;
		}

		if (Combat)
		{
			if (HasAuthority())
			{
				Combat->EquipWeapon(OverlappingWeapon);
			}
			else
			{
				Combat->Server_EquipButtonPressed();
			}
		}
	}
}

void ABaseCharacter::Sprint_InputActivation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (EnhancedMovement->CanSprint() && IsLocallyControlled())
	{
		EnhancedMovement->SetSprintInput(true);
	}
}

void ABaseCharacter::Sprint_InputDeactivation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (EnhancedMovement->IsSprinting() && IsLocallyControlled())
	{
		EnhancedMovement->SetSprintInput(false);
	}
}

void ABaseCharacter::CrouchInput_Activation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (EnhancedMovement->CanCrouch())
	{
		if (GetLocalRole() <= ROLE_Authority)
		{
			Crouch();
			Server_CrouchInput_Activation(Value);
		}
	}
}

void ABaseCharacter::CrouchInput_Deactivation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (GetLocalRole() <= ROLE_Authority)
	{
		if (bIsCrouched)
		{
			UnCrouch();
			Server_CrouchInput_Deactivation(Value);
		}
	}
}

void ABaseCharacter::Server_CrouchInput_Activation_Implementation(const FInputActionValue& Value)
{
	CrouchTransformLocation = FVector(0.0, 0.0, -40.0f);
	Multicast_Crouch(CrouchTransformLocation);
}

void ABaseCharacter::Server_CrouchInput_Deactivation_Implementation(const FInputActionValue& Value)
{
	if (bIsCrouched)
	{
		CrouchTransformLocation = FVector(0.0, 0.0, -89.0f);
		Multicast_Crouch(CrouchTransformLocation);
	}
}

void ABaseCharacter::Multicast_Crouch_Implementation(const FVector& NewLocation)
{
	CrouchTransformLocation = NewLocation;
	Head->SetRelativeLocation(CrouchTransformLocation);
}



void ABaseCharacter::SlideInput_Activation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (IsLocallyControlled())
	{
		if (EnhancedMovement->CanSlide())
		{
			EnhancedMovement->SetSlideInput(true);
		}
	}
}

void ABaseCharacter::SlideInput_Deactivation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (IsLocallyControlled())
	{
		if (EnhancedMovement->IsSliding())
		{
			EnhancedMovement->SetSlideInput(false);
		}
	}
}
void ABaseCharacter::AutoRun_Toggle()
{
	if (GameplayIsDisabled()) return;

	bIsAutoRunEnabled = !bIsAutoRunEnabled;

	if (bIsAutoRunEnabled)
	{
		if (CanAutoRun != true)
		{
			CanAutoRun = true;
		}
	}
	else
	{
		if (CanAutoRun)
		{
			CanAutoRun = false;
		}
	}
}
#pragma endregion | Input |

#pragma region | Jump System|
void ABaseCharacter::Jump()
{
	if (GameplayIsDisabled()) return;

	if (IsLocallyControlled())
	{
		if (bCustomJump != true)
		{
			bCustomJump = true;
		}

		Super::Jump();
		if (!IsLocallyControlled())
		{
			bPressedJump = false;
		}
	}
}

void ABaseCharacter::StopJumping()
{
	if (GameplayIsDisabled()) return;

	bCustomJump = false;
	Super::StopJumping();
}

#pragma endregion  | Jump System |

#pragma region    | Prone |
void ABaseCharacter::ProneInput(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (EnhancedMovement->IsSprinting() || EnhancedMovement->Velocity.Size() > 33.333f)
	{
		return;
	}

	if (ProneButtonAvaliable)
	{
		if (EnhancedMovement->GetIsProning() && EnhancedMovement->IsControllerWantsToProne())
		{
			EnhancedMovement->SetProneInput(false);
		}
		else if (EnhancedMovement->CanProne() && !EnhancedMovement->IsControllerWantsToProne())
		{
			EnhancedMovement->SetProneInput(true);
		}
		ProneButtonAvaliable = false;
		ProneTimerGate = true;
	}
}

void ABaseCharacter::ProneInputReleased(const FInputActionValue& Value)
{
	if (GameplayIsDisabled()) return;

	if (ProneTimerGate)
	{
		ProneTimerGate = false;
		float PressProneButtonDelay = 1.5f;
		GetWorldTimerManager().SetTimer(ProneTimerHandle, this, &ABaseCharacter::PressButtonRestart, PressProneButtonDelay, false);
	}
}

void ABaseCharacter::PressButtonRestart()
{
	ProneButtonAvaliable = true;
}

#pragma endregion | Prone |
FCollisionQueryParams ABaseCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

#pragma region | Weapons | 
void ABaseCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	OverlappingWeapon = Weapon;
}

AWeapon* ABaseCharacter::GetEquippedWeapon()
{
	if (Combat == nullptr) return nullptr;

	return Combat->EquippedWeapon;
}

void ABaseCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->SetDisplayEquipWeapon(true);
	}
	if (LastWeapon)
	{
		LastWeapon->SetDisplayEquipWeapon(false);
	}
}
#pragma endregion

#pragma region | Hit && Damage && Health |
void ABaseCharacter::Elimination()
{
	SetCharacterDead(true);

	if (Combat && Combat->EquippedWeapon)
	{
		Combat->EquippedWeapon->DropWeapon();
	}

	// Disable Character's Collision
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Disable Character's movement and inputs.
	EnhancedMovement->DisableMovement();
	EnhancedMovement->StopMovementImmediately();

	// Disable Character's movement inputs.
	if (!GameplayIsDisabled())
	{
		DisableGameplay(true);
	}
	
	Multicast_Elimination();

	// Set Timer Function
	GetWorldTimerManager().SetTimer
	(
		EliminationTimer,
		this,
		&ABaseCharacter::EliminationTimerFinished,
		EliminationTimerDelay
	);
}

void ABaseCharacter::EliminationTimerFinished()
{
	ACoreGameMode* CoreGameMode = GetWorld()->GetAuthGameMode<ACoreGameMode>();

	if (CoreGameMode)
	{
		CoreGameMode->RequestRespawn(this, Controller);
	}
	SetCharacterDead(false);

	bool SniperRifleIsValid =
		IsLocallyControlled()	&&
		IsAiming()				&&
		GetEquippedWeapon()		&&
		GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_SniperRifle;

	if (SniperRifleIsValid)
	{
		ShowSniperScopeWidget(false);
	}
}

void ABaseCharacter::Multicast_Elimination_Implementation()
{
	if (PlayerControllerManager)
	{
		PlayerControllerManager->SetHUDWeaponAmmo(0);
	}

	// Set the character to ragdoll state
	SetRagdollValues(true, ECollisionEnabled::PhysicsOnly);

	if (DissolveBodyMaterialInstance)
	{
		DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveBodyMaterialInstance, this);

		GetTorsoMesh()	->SetMaterial(0, DynamicDissolveMaterialInstance);
		GetLegsMesh()	->SetMaterial(0, DynamicDissolveMaterialInstance);
		GetArmsMesh()	->SetMaterial(0, DynamicDissolveMaterialInstance);
		GetHandsMesh()	->SetMaterial(0, DynamicDissolveMaterialInstance);
		GetBagMesh()	->SetMaterial(0, DynamicDissolveMaterialInstance);

		if (DissolveHeadMaterialInstance)
		{
			GetHeadMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);
		}

		if (DissolveHairMaterialInstance)
		{
			GetHairMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);
		}
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), 0.55f);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Glow"), 200.0f);
	}
	StartDissolve();

	// Disable Character's movement inputs.
	if (!GameplayIsDisabled())
	{
		DisableGameplay(true);
	}
	if (Combat)
	{
		Combat->SetFireButtonTrigger(false);
	}
	// Spawn EliminationBot
	if (EliminationBotEffect)
	{
		FVector EliminationBotSpawnPoint(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200.0f);
		EliminationBotComponent = UGameplayStatics::SpawnEmitterAtLocation
		(
			GetWorld(),
			EliminationBotEffect,
			EliminationBotSpawnPoint,
			GetActorRotation()
		);
	}

	if (EliminationBotSound)
	{
		UGameplayStatics::SpawnSoundAtLocation
		(
			this,
			EliminationBotSound,
			GetActorLocation()
		);
	}
}

void ABaseCharacter::SetRagdollValues(bool SimulatePhysics, ECollisionEnabled::Type NewType)
{
	GetHairMesh()	->SetSimulatePhysics(SimulatePhysics);
	GetHeadMesh()	->SetSimulatePhysics(SimulatePhysics);
	GetTorsoMesh()	->SetSimulatePhysics(SimulatePhysics);
	GetLegsMesh()	->SetSimulatePhysics(SimulatePhysics);
	GetArmsMesh()	->SetSimulatePhysics(SimulatePhysics);
	GetHandsMesh()	->SetSimulatePhysics(SimulatePhysics);
	GetBagMesh()	->SetSimulatePhysics(SimulatePhysics);
	GetHairMesh()	->SetCollisionEnabled(NewType);
	GetHeadMesh()	->SetCollisionEnabled(NewType);
	GetTorsoMesh()	->SetCollisionEnabled(NewType);
	GetLegsMesh()	->SetCollisionEnabled(NewType);
	GetArmsMesh()	->SetCollisionEnabled(NewType);
	GetHandsMesh()	->SetCollisionEnabled(NewType);
	GetBagMesh()	->SetCollisionEnabled(NewType);
}

void ABaseCharacter::Destroyed()
{
	Super::Destroyed();

	if (EliminationBotComponent)
	{
		EliminationBotComponent->DestroyComponent();
	}

	ACoreGameMode* CoreGameMode = Cast<ACoreGameMode>(UGameplayStatics::GetGameMode(this));

	bool bMatchNotInProgress = CoreGameMode && CoreGameMode->GetMatchState() != MatchState::InProgress;

	if (Combat && Combat->EquippedWeapon && bMatchNotInProgress)
	{
		Combat->EquippedWeapon->Destroy();
	}
}

void ABaseCharacter::UpdateDissolveMaterial(float DissolveValue)
{
	if (DynamicDissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), DissolveValue);
	}
}

void ABaseCharacter::StartDissolve()
{
	DissolveTrack.BindDynamic(this, &ABaseCharacter::UpdateDissolveMaterial);

	if (DissolveCurve && DissolveTimeline)
	{
		DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTrack);
		DissolveTimeline->Play();
	}
}

void ABaseCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	UpdateHUDHealth();
	if (Combat)
	{
		Combat->PlayHitReactionMontage();
	}
	if (Health == 0.0f)
	{
		ACoreGameMode* CoreGameMode = GetWorld()->GetAuthGameMode<ACoreGameMode>();
		if (CoreGameMode)
		{
			PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(Controller) : PlayerControllerManager;

			APlayerControllerManager* AttackerController = Cast<APlayerControllerManager>(InstigatorController);
			CoreGameMode->PlayerEliminated(this, PlayerControllerManager, AttackerController);
		}
	}
	
}

void ABaseCharacter::UpdateHUDHealth()
{
	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(Controller) : PlayerControllerManager;

	if (HasAuthority())
	{
		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDHealth(Health, MaxHealth);
		}
		else
		{
			LOG_WARNING("PlayerControllerManager HasAuthority NULL");
		}
	}
	else
	{
		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDHealth(Health, MaxHealth);
		}
		else
		{
			LOG_WARNING("PlayerControllerManager HasAuthority-Else NULL");
		}
	}
}

void ABaseCharacter::OnRep_Health()
{
	UpdateHUDHealth();
	if (Combat)
	{
		Combat->PlayHitReactionMontage();
	}
}
#pragma endregion

#pragma region | Getter And Setters |

FVector ABaseCharacter::GetHitTarget() const
{
	if (Combat == nullptr) return FVector();

	return Combat->HitTarget;
}
#pragma endregion

#pragma region | HUD |

void ABaseCharacter::PollInitialize()
{

	if (PlayerStateManager == nullptr)
	{
		CastPlayerInputComponent();
		PlayerStateManager = GetPlayerState<APlayerStateManager>();
		if (PlayerStateManager)
		{
			PlayerStateManager->AddToScore(0.0f);
			PlayerStateManager->AddToDefeats(0.0f);
		}
	}
}
#pragma endregion


ECombatTypes ABaseCharacter::GetCombatTypes() const
{
	if (Combat == nullptr) return ECombatTypes::ECT_MAX;
	return Combat->CombatTypes;
}