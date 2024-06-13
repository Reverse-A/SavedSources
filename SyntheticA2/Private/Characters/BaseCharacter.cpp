// @2023 "Age Of Reverse" Story. All rights reversed. 
// | Reverse - A Company.|

#include "Characters/BaseCharacter.h"

#include "Animation/AnimationInstance.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraConfiguration.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "Components/EnhancedMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerController/PlayerControllerManager.h"
#include "Sound/SoundCue.h"

#include "Weapon/Weapon.h"
#include "Net/UnrealNetwork.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"


#pragma region Constructor
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UEnhancedMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	bNetLoadOnClient		= true;
	bNetUseOwnerRelevancy	= false;
	NetDormancy				= DORM_Awake;
	NetCullDistanceSquared	= 900000000.0f;
	NetUpdateFrequency		= 66.0f;
	MinNetUpdateFrequency	= 33.0f;
	NetPriority				= 3.0f;


	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check			  (CapsuleComp);

	CapsuleComp	->InitCapsuleSize(35.0f, 89.95f);
	CapsuleComp	->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()	->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Hair	= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Head	= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));

	Legs	= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Arms	= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Hands	= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));

	Hair	->SetCollisionObjectType(ECC_SkeletalMesh);
	Head	->SetCollisionObjectType(ECC_SkeletalMesh);
	Legs	->SetCollisionObjectType(ECC_SkeletalMesh);
	Arms	->SetCollisionObjectType(ECC_SkeletalMesh);
	Hands	->SetCollisionObjectType(ECC_SkeletalMesh);

	Hair	->SetCollisionResponseToChannel(ECC_VisibilityBlock);
	Head	->SetCollisionResponseToChannel(ECC_VisibilityBlock);
	Legs	->SetCollisionResponseToChannel(ECC_VisibilityBlock);
	Arms	->SetCollisionResponseToChannel(ECC_VisibilityBlock);
	Hands	->SetCollisionResponseToChannel(ECC_VisibilityBlock);

	Head	->SetupAttachment(CharacterMesh);
	Hair	->SetupAttachment(CharacterMesh);
	Legs	->SetupAttachment(CharacterMesh);
	Arms	->SetupAttachment(CharacterMesh);
	Hands	->SetupAttachment(CharacterMesh);

	/* Camera Properties */
	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPPSpringArm"));
	FirstPersonCamera	 = CreateDefaultSubobject<UCameraComponent>	  (TEXT("FirstPersonCamera"));
	ThirdPersonCamera	 = CreateDefaultSubobject<UCameraComponent>	  (TEXT("ViewCamera"));

	ThirdPersonSpringArm->SetupAttachment(CharacterMesh);
	ThirdPersonSpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 160.0f));
	ThirdPersonSpringArm->TargetArmLength		  = 400.f;
	ThirdPersonSpringArm->bUsePawnControlRotation = true;
	ThirdPersonSpringArm->SocketOffset		      = FVector(0.0f, 65.0f, 30.0f);
	ThirdPersonSpringArm->TargetOffset			  = FVector::ZeroVector;
	ThirdPersonSpringArm->bUsePawnControlRotation = true;
	ThirdPersonSpringArm->bInheritPitch			  = true;
	ThirdPersonSpringArm->bInheritYaw			  = true;
	ThirdPersonSpringArm->bInheritRoll		      = true;

	ThirdPersonCamera	 ->SetupAttachment(ThirdPersonSpringArm);
	ThirdPersonCamera	 ->SetProjectionMode(ECameraProjectionMode::Perspective);
	ThirdPersonCamera	 ->bUsePawnControlRotation = false;
	ThirdPersonCamera	 ->SetFieldOfView(80.0f);

	FirstPersonCamera	 ->SetupAttachment(Head, FName("head"));
	FirstPersonCamera	 ->bUsePawnControlRotation = true;
	FirstPersonCamera	 ->SetRelativeLocation(FVector(0.0f, 20.0f, 0.0f));
	FirstPersonCamera	 ->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	FirstPersonCamera	 ->bAutoActivate = false;

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw	= false;
	bUseControllerRotationRoll	= false;
	BaseEyeHeight				= 80.0f;
	CrouchedEyeHeight			= 50.0f;

	// Class components
	CameraConfigurationComponent = CreateDefaultSubobject<UCameraConfiguration>(TEXT("CameraComponent"));

	EnhancedMovementComponent = Cast<UEnhancedMovementComponent>(GetCharacterMovement());
	EnhancedMovementComponent->bOrientRotationToMovement = true;
	EnhancedMovementComponent->SetIsReplicated(true);
	EnhancedMovementComponent->RotationRate = FRotator(0.0f, 400.0f, 0.0f);

}
#pragma endregion

#pragma region General Functions
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CameraConfigurationComponent = CameraConfigurationComponent == nullptr ? Cast<UCameraConfiguration>(CameraConfigurationComponent) : CameraConfigurationComponent;
	EnhancedMovementComponent = EnhancedMovementComponent		== nullptr ? Cast<UEnhancedMovementComponent>(GetCharacterMovement()) : EnhancedMovementComponent;	
	EnhancedInputLocalPlayerSubsystem();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (OverlappingWeapon)
	{
		LOG_WARNING("OverlappingWeapon")
	}
	else
	{
		LOG_WARNING("OverlappingWeapon == nullptr")

	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}
void ABaseCharacter::CastComponentClasses()
{

}
#pragma endregion

#pragma region Controller
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(CameraSwitchAction,		ETriggerEvent::Started,		this, &ABaseCharacter::InputSwitchCamera);

		EnhancedInputComponent->BindAction(LookInputAction,			ETriggerEvent::Triggered,	this, &ABaseCharacter::InputLook);
		EnhancedInputComponent->BindAction(MoveInputAction,			ETriggerEvent::Triggered,	this, &ABaseCharacter::InputMovement_Activation);
		EnhancedInputComponent->BindAction(MoveInputAction,			ETriggerEvent::Completed,	this, &ABaseCharacter::InputMovement_DeActivation);

		EnhancedInputComponent->BindAction(InteractionInputAction,	ETriggerEvent::Started,		this, &ABaseCharacter::InputInteraction);

		EnhancedInputComponent->BindAction(SprintInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::InputSprint);

		EnhancedInputComponent->BindAction(CrouchInputAction,		ETriggerEvent::Triggered,	this, &ABaseCharacter::InputCrouch_Activation);
		EnhancedInputComponent->BindAction(CrouchInputAction,		ETriggerEvent::Completed,	this, &ABaseCharacter::InputCrouch_DeActivation);

	}
}

#pragma endregion

#pragma region InputFunctions
void ABaseCharacter::EnhancedInputLocalPlayerSubsystem()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void ABaseCharacter::InputSwitchCamera(const FInputActionValue& Value)
{
	CameraConfigurationComponent->SwitchCamera();
}

void ABaseCharacter::InputMovement_Activation(const FInputActionValue& Value)
{
	//if (EnhancedMovement->IsMantling() || GameplayIsDisabled())
	//{
	//	return;
	//}

	//if (CanAutoRun)
	//{
	//	if (MovementDirectionY != 0 || MovementDirectionX != 0)
	//	{
	//		if (CanAutoRun)
	//		{
	//			CanAutoRun = false;
	//		}
	//		if (bIsAutoRunEnabled)
	//		{
	//			bIsAutoRunEnabled = false;
	//		}
	//	}
	//}

	MovementInputActivated = true;
	if (CameraConfigurationComponent->IsCameraViewPerspective(ECameraViewPerspective::ECVP_ThirdPerson))
	{
		if (Controller != NULL)
		{
			if (IsAiming())
			{
				MovementVector = Value.Get<FVector2D>();
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

				const FVector MovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);
				AddMovementInput(MovementDirection);
				MovementDirectionX = MovementVector.X;
				MovementDirectionY = MovementVector.Y;
			}
			else
			{
				MovementVector = Value.Get<FVector2D>();
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

				const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				//if (EnhancedMovement->IsSliding())
				//{
				//	ForwardDirection = FVector::ZeroVector;
				//}

				FVector MovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);

				AddMovementInput(MovementDirection);
				MovementDirectionX = MovementVector.X;
				MovementDirectionY = MovementVector.Y;
			}
		}
	}
	else if (CameraConfigurationComponent->IsCameraViewPerspective(ECameraViewPerspective::ECVP_FirstPerson))
	{
		if (Controller != NULL)
		{
			MovementVector = Value.Get<FVector2D>();

			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);

			MovementDirectionX = MovementVector.X;
			MovementDirectionY = MovementVector.Y;
		}
	}
}

void ABaseCharacter::InputMovement_DeActivation(const FInputActionValue& Value)
{
	MovementInputActivated = false;
	//if (GameplayIsDisabled()) return;

	//if (IsLocallyControlled())
	//{
	//	Server_SetMovementInput(false);
	//}
}

void ABaseCharacter::InputLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	float ScaledPitchInput	= LookAxisVector.Y * -1.0f * MouseSensitivity;
	float ScaledYawInput	= LookAxisVector.X *		 MouseSensitivity;

	AddControllerPitchInput	(ScaledPitchInput);
	AddControllerYawInput	(ScaledYawInput);
}

void ABaseCharacter::InputInteraction(const FInputActionValue& Value)
{
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
				LOG_WARNING("AUTHORITY EQUIPPED")
			}
			else
			{
				Combat->Server_EquipButtonPressed();
				LOG_WARNING("NON AUTHORITY EQUIPPED")
			}
		}
	}
}

void ABaseCharacter::InputReload(const FInputActionValue& Value)
{

}

void ABaseCharacter::InputAim_Activation(const FInputActionValue& Value)
{
	//if (GameplayIsDisabled()) return;
	//if (Combat->EquippedWeapon == nullptr) return;

	if (CanAim())
	{
		if (bCached_Aiming != true)
		{
			bCached_Aiming = true;
		}
		SetAim(true);
	}
}

void ABaseCharacter::InputAim_DeActivation(const FInputActionValue& Value)
{
	//if (GameplayIsDisabled()) return;

	//if (Combat->IsFireButtonPressed())
	//{
	//	bCached_Aiming = false;
	//	return;
	//}
	if (IsAiming())
	{
		if (bCached_Aiming != false)
		{
			bCached_Aiming = false;
		}
		SetAim(false);
	}
}

void ABaseCharacter::InputFire(const FInputActionValue& Value)
{

}

void ABaseCharacter::InputJump(const FInputActionValue& Value)
{

}

void ABaseCharacter::InputCrouch_Activation(const FInputActionValue& Value)
{
	EnhancedMovementComponent->SetCrouchInput(true);
}

void ABaseCharacter::InputCrouch_DeActivation(const FInputActionValue& Value)
{
	EnhancedMovementComponent->SetCrouchInput(false);
}

void ABaseCharacter::InputProne(const FInputActionValue& Value)
{
}

void ABaseCharacter::InputSprint(const FInputActionValue& Value)
{
	if (EnhancedMovementComponent->CanSprint())
	{
		EnhancedMovementComponent->SwitchSprint();
		EnhancedMovementComponent->SetSprintInput(true);
	}	
}



void ABaseCharacter::InputSlide(const FInputActionValue& Value)
{

}

void ABaseCharacter::InputDash(const FInputActionValue& Value)
{

}

void ABaseCharacter::InputAutoRun(const FInputActionValue& Value)
{

}
bool ABaseCharacter::CanAim()
{
	return !IsAiming();
}
#pragma endregion





#pragma region Weapon
void ABaseCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	OverlappingWeapon = Weapon;
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


#pragma region IgnoredParamaters
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