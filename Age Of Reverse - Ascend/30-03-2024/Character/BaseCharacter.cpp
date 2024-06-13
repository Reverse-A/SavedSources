// @2023 All rights reversed by Reverse-Alpha Studios


#include "BaseCharacter.h"
#include "ShortDebugMacros.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "Ascend/Character/CharacterDefaultAnimInstance.h"

#include "Ascend/Component/ArsenalComponent.h"
#include "Ascend/Component/CombatSystemComponent.h"
#include "Ascend/Component/ViewInterfaceComponent.h"
#include "Ascend/Component/AttributeComponent.h"

#include "Ascend/Weapon/BaseWeapon.h"
#include "Ascend/Weapon/MeleeWeapon.h"
#include "Ascend/Weapon/RangedWeapon.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

#include "Engine/DamageEvents.h"


static FName NAME_ViewCamera(TEXT("ViewCamera"));


ABaseCharacter::ABaseCharacter()
{
	// Avoid ticking characters if possible.
	//PrimaryActorTick.bCanEverTick			= false;
	PrimaryActorTick.bStartWithTickEnabled	= true;

	SetupCharacterCapsuleComponent();
	SetupCharacterMeshComponents();
	SetupPlayerCameraComponent();
	SetupCharacterMovementComponent();

	CreateSubobjectComponents();

	SetupStimulusSource();
}



void ABaseCharacter::SetupCharacterCapsuleComponent()
{
	// Get the "CapsuleComponent attached to this character.
	CharacterCapsule = GetCapsuleComponent();
	// Ensure that the Capsule component exists.
	check(CharacterCapsule);
	// Set the Capsule Component as the root component for the character.
	SetRootComponent(CharacterCapsule);
	// Initialize the size of the capsule (radius and half-height).
	CharacterCapsule->InitCapsuleSize(35.0f, 89.95f);
	// Set collision response for the camera to ignore collisions with this capsule.
	CharacterCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::SetupCharacterMeshComponents()
{
	// Initializes member variables with corresponding skeletal mesh components.
	CharacterMesh = GetMesh();
	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));

	// Ensures that all created skeletal mesh components are valid.
	check(CharacterMesh);
	check(Hair);
	check(Head);
	check(Legs);
	check(Arms);
	check(Hands);

	// Attaches each skeletal mesh component to the "CharacterMesh".
	Hair->SetupAttachment(CharacterMesh);
	Head->SetupAttachment(CharacterMesh);
	Legs->SetupAttachment(CharacterMesh);
	Arms->SetupAttachment(CharacterMesh);
	Hands->SetupAttachment(CharacterMesh);

	// Disables casting of hidden shadows for all skeletal mesh components.
	CharacterMesh->bCastHiddenShadow = false;
	Hair->bCastHiddenShadow			 = false;
	Head->bCastHiddenShadow			 = false;
	Legs->bCastHiddenShadow			 = false;
	Arms->bCastHiddenShadow			 = false;
	Hands->bCastHiddenShadow		 = false;

	// Sets all skeletal mesh components to be visible in-game.
	Hair->SetHiddenInGame(false);
	Head->SetHiddenInGame(false);
	Legs->SetHiddenInGame(false);
	Arms->SetHiddenInGame(false);
	Hands->SetHiddenInGame(false);

	// Sets up attachment to the CharacterMesh with a specific relative location and rotation
	FVector  MeshRelativeLocation = FVector(0.0f, 0.0f, -93.0f);
	FRotator MeshRelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

	// Sets up attachment of the CharacterMesh to the CharacterCapsule with specific relative locationn and rotation.
	CharacterMesh->SetupAttachment(CharacterCapsule);
	CharacterMesh->SetRelativeLocation(MeshRelativeLocation);
	CharacterMesh->SetRelativeRotation(MeshRelativeRotation);
	CharacterMesh->SetCastHiddenShadow(false);

	//Set the leader mesh for other skeletal mesh components as follows:
	GetHairMesh()->SetLeaderPoseComponent(GetMesh());
	GetHeadMesh()->SetLeaderPoseComponent(GetMesh());
	GetArmsMesh()->SetLeaderPoseComponent(GetMesh());
	GetHandsMesh()->SetLeaderPoseComponent(GetMesh());
	GetLegsMesh()->SetLeaderPoseComponent(GetMesh());
}

void ABaseCharacter::SetupPlayerCameraComponent()
{
	// Create a default camera component and attach it to the character's mesh
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(NAME_ViewCamera);
	PlayerCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("spine_03"));
	// Set the relative location of the camera.
	FVector CameraRelativeLocation(25.0f, 17.0f, 0.0f);
	PlayerCamera->SetRelativeLocation(CameraRelativeLocation);
	// Set the relative rotation of the camera.
	FRotator CameraRelativeRotation(0.0f, 90.0f, -90.0f);
	PlayerCamera->SetRelativeRotation(CameraRelativeRotation);
	// Set the projection mode of the camera to perpective.
	PlayerCamera->SetProjectionMode(ECameraProjectionMode::Perspective);
	// Set the filed of view of the camera.
	PlayerCamera->FieldOfView = 110.0f;
	// Lock the camera to the head-mounted display(HMD).
	PlayerCamera->bLockToHmd = true;
	// Use the field of view for level of detail (LOD) calculations.
	PlayerCamera->bUseFieldOfViewForLOD = true;
	// Allow drawing of frustum for debugging. 
	PlayerCamera->bDrawFrustumAllowed = true;
	// Hide the camera mesh in-game.
	PlayerCamera->bCameraMeshHiddenInGame = true;
}

void ABaseCharacter::SetupCharacterMovementComponent()
{
	// Configure character movement parameters such as crouched height, maximum walk speed, swim speed, and related settings.
	CharacterMovementComp = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	CharacterMovementComp->SetCrouchedHalfHeight(60.0f);
	CharacterMovementComp->MaxWalkSpeed						= 300.0f;
	CharacterMovementComp->MaxSwimSpeed						= 150.0f;
	CharacterMovementComp->MaxWalkSpeedCrouched				= 150.0f;
	CharacterMovementComp->bCanWalkOffLedgesWhenCrouching	= true;
	CharacterMovementComp->NavAgentProps.bCanCrouch			= true;

	// Control whether the character's rotation follows the controller's rotation for pitch and yaw.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw	= true;
	bUseControllerRotationRoll	= false;
}


void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatSystem)
	{
		CombatSystem->SetBaseCharacter(this);
	}

	if (ArsenalSystem)
	{
		ArsenalSystem->SetBaseCharacter(this);
	}

	if (ViewInterface)
	{
		ViewInterface->SetBaseCharacter(this);
	}

	if (AttributeComponent)
	{
		AttributeComponent->SetBaseCharacter(this);
	}
}


void ABaseCharacter::CreateSubobjectComponents()
{
	static FName NAME_CombatSystem		(TEXT("CombatSystem"));
	static FName NAME_ArsenalSystem		(TEXT("ArsenalComponent"));
	static FName NAME_ViewInterface		(TEXT("ViewInterfaceComponent"));
	static FName NAME_Attribute			(TEXT("Attribute Component"));

	CombatSystem		= CreateDefaultSubobject<UCombatSystemComponent>	(NAME_CombatSystem);
	ArsenalSystem		= CreateDefaultSubobject<UArsenalComponent>			(NAME_ArsenalSystem);
	ViewInterface		= CreateDefaultSubobject<UViewInterfaceComponent>	(NAME_ViewInterface);
	AttributeComponent	= CreateDefaultSubobject<UAttributeComponent>		(NAME_Attribute);
}



void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterAnimInstance = Cast<UCharacterDefaultAnimInstance>(GetMesh()->GetAnimInstance());
	if (CharacterAnimInstance)
	{
		CharacterAnimInstance->SetWeaponType(EWeaponType::EWT_Fists);
	}
}
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ResetLean();
}


#pragma region PlayerInputs
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction,			ETriggerEvent::Triggered,	this, &ABaseCharacter::InputFunctionMovement);
		EnhancedInputComponent->BindAction(MoveInputAction,			ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionMovementC);
		EnhancedInputComponent->BindAction(LookInputAction,			ETriggerEvent::Triggered,	this, &ABaseCharacter::InputFunctionLook);
		EnhancedInputComponent->BindAction(JumpInputAction,			ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionJump);
		EnhancedInputComponent->BindAction(CrouchInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionCrouch);
		EnhancedInputComponent->BindAction(CrouchInputAction,		ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionCrouch);
		EnhancedInputComponent->BindAction(ProneInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionProne);
		EnhancedInputComponent->BindAction(SprintInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionSprint);
		EnhancedInputComponent->BindAction(SprintInputAction,		ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionSprintEnd);

		EnhancedInputComponent->BindAction(AttackInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionAttack);
		EnhancedInputComponent->BindAction(AttackInputAction,		ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionAttackEnd);

		EnhancedInputComponent->BindAction(LeanRightInputAction,	ETriggerEvent::Triggered,	this, &ABaseCharacter::InputFunctionLean);
		EnhancedInputComponent->BindAction(LeanRightInputAction,	ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionLeanCompleted);
		EnhancedInputComponent->BindAction(InteractionInputAction,	ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionInteraction);

		EnhancedInputComponent->BindAction(AimInputAction,			ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionAim);
		EnhancedInputComponent->BindAction(AimInputAction,			ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionAimEnd);

		EnhancedInputComponent->BindAction(FMSwitchInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionWeaponFiringModeSwitch);

		EnhancedInputComponent->BindAction(DebugInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::DebugInput);
	}
}

void ABaseCharacter::DebugInput()
{

}

/*Dynamics*/
void ABaseCharacter::InputFunctionMovement(const FInputActionValue& Value)
{
	if (Controller == nullptr) 	return;

	FVector2D MovementValue = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MovementValue.Y);
	AddMovementInput(GetActorRightVector(), MovementValue.X);

	CharacterForwardDirection = MovementValue.Y;
	CharacterRightDirection   = MovementValue.X;

	UpdateSpeedAndDirections();
}

void ABaseCharacter::InputFunctionMovementC(const FInputActionValue& Value)
{
	if (Controller == nullptr)
		return;

	FVector2D MovementValue = Value.Get<FVector2D>();

	CharacterForwardDirection = MovementValue.Y;
	CharacterRightDirection   = MovementValue.X;

	ResetSpeedAndDirections();
}

void ABaseCharacter::ControlCharacterMovementSpeed()
{
	float ProneSpeed	= 75.0f;
	float CrouchSpeed	= 150.0f;
	float JogSpeed		= 300.0f;
	float SprintSpeed	= 600.0f;
	float Divisor		= 2.0f;

	if (bCharacterProne)
	{
		if (bCharacterAiming && bCharacterAttacking)
		{
			CharacterMovementComp->MaxWalkSpeed = 0.0f;

		}
		else if (bCharacterAiming || bCharacterAttacking)
		{
			CharacterMovementComp->MaxWalkSpeed = 0.0f;
		}
		else
		{
			CharacterMovementComp->MaxWalkSpeed = ProneSpeed;
		}		
	}
	else if (bCharacterCrouching)
	{
		if (bCharacterAiming && bCharacterAttacking)
		{
			CharacterMovementComp->MaxWalkSpeed = ((CrouchSpeed / Divisor) / Divisor);
		}
		else if (bCharacterAiming || bCharacterAttacking)
		{
			CharacterMovementComp->MaxWalkSpeed = (CrouchSpeed / Divisor);
		}
		else
		{
			CharacterMovementComp->MaxWalkSpeed = CrouchSpeed;
		}
	}
	else if (bCharacterSprinting)
	{
		if (bCharacterAiming && bCharacterAttacking)
		{
			CharacterMovementComp->MaxWalkSpeed = ((SprintSpeed / Divisor) / Divisor);
		}
		else if (bCharacterAiming || bCharacterAttacking)
		{
			CharacterMovementComp->MaxWalkSpeed = (SprintSpeed / Divisor);
		}
		else
		{
			CharacterMovementComp->MaxWalkSpeed = SprintSpeed;
		}
	}
	else
	{
		CharacterMovementComp->MaxWalkSpeed = JogSpeed;
	}
}

void ABaseCharacter::InputFunctionLook(const FInputActionValue& Value)
{
	if (Controller == nullptr)
		return;

	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	float ScaledPitchInput		= LookAxisValue.Y * VerticalSensitivity;
	float ScaledYawInput		= LookAxisValue.X * HorizontalSensitivity;

	const float Negate			= -1.0f;

	AddControllerPitchInput(ScaledPitchInput * Negate);
	AddControllerYawInput  (ScaledYawInput);

	CameraAimPitch += ScaledPitchInput;
	CameraAimPitch = FMath::Clamp(CameraAimPitch, -30.0f, 30.0f);
	RotateBonePitch = CameraAimPitch;

	float MultipliedValueX = ScaledYawInput * 0.25f;

	if (CharacterRightDirection != 0 || CharacterForwardDirection != 0)
	{
		MultipliedValueX	= 0.0f;
		ScaledYawInput		= 0.0f;
		RotateBoneYaw		= 0.0f;
		CameraAimYaw		= 0.0f;
	}

	CameraAimYaw = FMath::Clamp(CameraAimYaw + MultipliedValueX, -22.5f, 22.5f);
	RotateBoneYaw = CameraAimYaw;
}

void ABaseCharacter::InputFunctionLean(const FInputActionValue& Value)
{
	if (Controller == nullptr) return;
	if (bCharacterSprinting == true || bCharacterProne == true ) return;
	if (LeanRightRoll >= 10.0f || LeanRightRoll <= -10.0f) return;

	if (bWantsToLean != true)
		bWantsToLean = true;
	
	if (bWantsToLean)
	{
		float LeanValue = Value.Get<float>();

		float InRangeA	= -1.0f;
		float InRangeB	= 1.0f;
		float OutRangeA = -10.0f;
		float OutRangeB = 10.0f;
		float InterpolateSpeed = 7.50f;

		LeanRightRoll = FMath::FInterpTo
		(
			LeanRightRoll,
			FMath::GetMappedRangeValueClamped
			(
				FVector2D(InRangeA, InRangeB),
				FVector2D(OutRangeA, OutRangeB),
				LeanValue
			),
			GetWorld()->GetDeltaSeconds(),
			InterpolateSpeed
		);

		bCharacterLeaning = bWantsToLean;
	}
}

void ABaseCharacter::InputFunctionLeanCompleted()
{
	if (Controller == nullptr) return;

	if (bWantsToLean != false)
	{
		bWantsToLean = false;
		bCharacterLeaning = bWantsToLean;
	}
}

void ABaseCharacter::ResetLean()
{
	if (LeanRightRoll == 0 || bWantsToLean == true)
	{
		return;
	}
	else
	{
		if (bWantsToLean == false)
		{
			float InRangeA = -1.0f;
			float InRangeB = 1.0f;
			float OutRangeA = -10.0f;
			float OutRangeB = 10.0f;
			float Interpolatespeed = 10.0f;

			LeanRightRoll = FMath::FInterpTo
			(
				LeanRightRoll,
				0.0f,
				GetWorld()->GetDeltaSeconds(),
				Interpolatespeed
			);
		}
	}
}

/*Statics*/
void ABaseCharacter::InputFunctionJump()
{
	if (bCharacterCrouching == true || bCharacterProne == true ) return;
	bPressedJump = true;
	Super::Jump();
}

void ABaseCharacter::InputFunctionCrouch()
{
	if (CharacterMovementComp->IsFalling()) return;

	if (bCharacterSprinting == true)
	{
		LOG_TO_DESKTOP("TODO Slide() function could implementable");
		//TODO Slide() function could implementable.!
	}

	bWantsToCrouch = !bWantsToCrouch;

	if (bWantsToCrouch)
	{
		Crouch();
		bCharacterCrouching = bWantsToCrouch;
	}
	else
	{
		UnCrouch();
		bCharacterCrouching = bWantsToCrouch;
	}
	ControlCharacterMovementSpeed();
}
void ABaseCharacter::InputFunctionProne()
{
	if (CharacterMovementComp->IsFalling()) return;

	bWantsToProne = !bWantsToProne;
	if (bWantsToProne)
	{
		if (bCharacterSprinting == true)
		{
			LOG_TO_DESKTOP("Sprint to jumping prone function could implementable");
			//TODO SprintToProne() function could implementable.!
		}

		CharacterMovementComp->SetCrouchedHalfHeight(30.0f);
		Crouch();
		bCharacterProne = true;
		ControlCharacterMovementSpeed();
		LW("PRONE START");
	}
	else
	{
		if (bCharacterProne == true) bCharacterProne = false;
		UnCrouch();
		ControlCharacterMovementSpeed();
		LW("PRONE END");
	}
}

void ABaseCharacter::InputFunctionSprint()
{
	if (bCharacterAttacking || bCharacterAiming || bCharacterProne) return;

	if (bCharacterCrouching)
	{
		LOG_TO_DESKTOP("If the player sprint is requested in the crouch state, the character's movement component must be implemented by 'Inertia'.");
		//TODO AddInertiaConceptToWalkSpeed() function could implementable.!
	}
	bWantsToSprint = true;
	bCharacterSprinting = bWantsToSprint;
	ControlCharacterMovementSpeed();
}

void ABaseCharacter::InputFunctionSprintEnd()
{
	if (bAttackButtonPressed || bAimButtonPressed) return;

	bWantsToSprint = false;
	bCharacterSprinting = bWantsToSprint;
	ControlCharacterMovementSpeed();
}

void ABaseCharacter::InputFunctionInteraction()
{
	if (Controller == nullptr) return;

	if (OverlapWeapon != nullptr)
	{
		ArsenalSystem->ArrangeWeaponByTypesAndClasses(OverlapWeapon);
	}
}

void ABaseCharacter::InputFunctionAttack()
{
	if (CombatSystem && CombatSystem->CanAttack())
	{
		bAttackButtonPressed = true;
		CombatSystem->MeleeAttack(bAttackButtonPressed);
		check(CharacterAnimInstance);
		CharacterAnimInstance->SetIsAttacking(bAttackButtonPressed);
	}
	else if (CombatSystem && CombatSystem->CanFire())
	{
		bAttackButtonPressed = true;
		check(CharacterAnimInstance);
		CharacterAnimInstance->SetIsAttacking(bAttackButtonPressed);
		CombatSystem->SetFire(bAttackButtonPressed);
		CombatSystem->RangeAttack();
	}
	bCharacterAttacking = bAttackButtonPressed;
	ControlCharacterMovementSpeed();
}

void ABaseCharacter::InputFunctionAttackEnd()
{
	if (ArsenalSystem->GetPrimaryEquipWeapon() == nullptr) return;
	if (ArsenalSystem->GetPrimaryEquipWeapon()->GetWeaponType() != EWeaponType::EWT_Ranged) return;
	if (ArsenalSystem->GetRangedWeapon()->GetRangedWeaponProperties().GetWeaponFireMode() == EWeaponFiringMode::Automatic && bAttackButtonPressed == true)
	{
		bAttackButtonPressed = false;
		CombatSystem->SetFire(bAttackButtonPressed);
		CharacterAnimInstance->SetIsAttacking(bAttackButtonPressed);
	}
	bCharacterAttacking = bAttackButtonPressed;	
	ControlCharacterMovementSpeed();
}

void ABaseCharacter::InputFunctionAim()
{
	if (CombatSystem && CombatSystem->CanAim())
	{
		bAimButtonPressed = true;
		CharacterAnimInstance->SetIsAiming(bAimButtonPressed);
		bCharacterAiming = bAimButtonPressed;
	}
	ControlCharacterMovementSpeed();
}

void ABaseCharacter::InputFunctionAimEnd()
{
	if (GetAimButtonPressed())
	{
		bAimButtonPressed = false;
		CharacterAnimInstance->SetIsAiming(bAimButtonPressed);
		bCharacterAiming = bAimButtonPressed;
	}
	ControlCharacterMovementSpeed();
}

void ABaseCharacter::InputFunctionWeaponFiringModeSwitch()
{
	if (!ArsenalSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Arsenal system not found"));
		return;
	}

	if (!ArsenalSystem->GetPrimaryEquipWeapon())
	{
		UE_LOG(LogTemp, Warning, TEXT("Primary equipped weapon not found"));
		return;
	}
		
	SwitchWeaponFiringMode(ArsenalSystem->GetPrimaryEquipWeapon());
}

void ABaseCharacter::SwitchWeaponFiringMode(ABaseWeapon* InputWeapon)
{
	if (InputWeapon == nullptr) return;

	ARangedWeapon* RangeWeapon = Cast<ARangedWeapon>(InputWeapon);

	if (!(RangeWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Rifle ||
		RangeWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Heavymachine ||
		RangeWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Submachine))
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon type is not switchable"));
		return;
	}

	if (RangeWeapon->GetRangedWeaponProperties().GetWeaponFireMode() == EWeaponFiringMode::SingleShot)
	{
		RangeWeapon->SetWeaponFireMode(EWeaponFiringMode::SemiAutomatic);
		UE_LOG(LogTemp, Warning, TEXT("Firing mode switched to SemiAutomatic"));
	}
	else if (RangeWeapon->GetRangedWeaponProperties().GetWeaponFireMode() == EWeaponFiringMode::SemiAutomatic)
	{
		RangeWeapon->SetWeaponFireMode(EWeaponFiringMode::Automatic);
		UE_LOG(LogTemp, Warning, TEXT("Firing mode switched to Automatic"));
	}
	else if (RangeWeapon->GetRangedWeaponProperties().GetWeaponFireMode() == EWeaponFiringMode::Automatic)
	{
		RangeWeapon->SetWeaponFireMode(EWeaponFiringMode::SingleShot);
		UE_LOG(LogTemp, Warning, TEXT("Firing mode switched to SingleShot"));
	}

}

void ABaseCharacter::InputFunctionEnterVehicle()
{

}

void ABaseCharacter::InputFunctionSwapWeapon()
{

}
void ABaseCharacter::InputFunctionReload()
{

}

void ABaseCharacter::InputFunctionSlide()
{
}
void ABaseCharacter::InputFunctionDash()
{
}

void ABaseCharacter::InputFunctionOption()
{
}
void ABaseCharacter::InputFunctionScore()
{
}

bool ABaseCharacter::DisableAim()
{
	if (ArsenalSystem)
	{
		if (ArsenalSystem->GetPrimaryEquipWeapon() != nullptr)
		{
			if (ArsenalSystem->GetPrimaryEquipWeapon()->GetWeaponType() == EWeaponType::EWT_Melee)
				return true;
		}
		
		if (ArsenalSystem->GetSecondaryEquipWeapon() != nullptr)
		{
			if (ArsenalSystem->GetSecondaryEquipWeapon()->GetWeaponType() == EWeaponType::EWT_Melee) 
				return true;
		}
	}
	return false;
}
#pragma endregion

#pragma region Movement
void ABaseCharacter::UpdateSpeedAndDirections()
{
	if (CharacterForwardDirection == 0)
	{
		ForwardSpeed = 0.0f;
	}
	else
	{
		float IncreaseSpeedbyForwardSprintSpeed = bWantsToSprint ? 300.0f : 0.0f;
		ForwardSpeed = CharacterForwardDirection > 0 ? 300.0f + IncreaseSpeedbyForwardSprintSpeed : -300.0f;
	}

	if (CharacterRightDirection == 0)
	{
		RightSpeed = 0.0f;
	}
	else
	{	
		float IncreaseSpeedbyRighhtSprintSpeed = bWantsToSprint ? 150.0f : 0.0f;
		RightSpeed   = CharacterRightDirection > 0 ? 150.0f + IncreaseSpeedbyRighhtSprintSpeed : -150.0f - IncreaseSpeedbyRighhtSprintSpeed;
	}
}
void ABaseCharacter::ResetSpeedAndDirections()
{
	if (CharacterForwardDirection == 0)
	{
		ForwardSpeed = 0.0f;
	}

	if (CharacterRightDirection == 0)
	{
		RightSpeed = 0.0f;
	}
}


#pragma endregion
#pragma region Damage-System
float ABaseCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	//ReceiveDamage(DamageAmount);
	//HealthBarWidget->SetBarValuePercent(AttributeComponent->GetHealth() / AttributeComponent->GetMaxHealth());
	UE_LOG(LogTemp, Warning, TEXT("DamageTaken: %f"), DamageAmount);
	return DamageAmount;

	return 0.0f;
}
void ABaseCharacter::ReceiveDamage(float Damage)
{
	// Create a default damage event
	FDamageEvent DamageEvent;

	// Get the instigator controller
	AController* InstigatorController = GetInstigatorController();

	// Call TakeDamage with the provided damage value and default damage event
	TakeDamage(Damage, DamageEvent, InstigatorController, this);
}

#pragma endregion
#pragma region AI-System

void ABaseCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));

	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}
#pragma endregion

