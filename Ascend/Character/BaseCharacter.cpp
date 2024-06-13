// @2023 All rights reversed by Reverse-Alpha Studios


#include "BaseCharacter.h"
#include "ShortDebugMacros.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"

#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "Ascend/Character/CharacterDefaultAnimInstance.h"

#include "Ascend/AI/AIEntityModule.h"

#include "Ascend/Component/ArsenalComponent.h"
#include "Ascend/Component/CombatSystemComponent.h"
#include "Ascend/Component/ViewInterfaceComponent.h"
#include "Ascend/Component/AttributeComponent.h"
#include "Ascend/Component/LevelSystemComponent.h"

#include "Ascend/Weapon/BaseWeapon.h"
#include "Ascend/Weapon/MeleeWeapon.h"
#include "Ascend/Weapon/RangedWeapon.h"

#include "Ascend/PlayerController/PlayerControllerModule.h"
#include "Ascend/PlayerState/PlayerStateModule.h"

#include "Ascend/GameMode/BaseGameMode.h"
#include "Ascend/GameState/GameSaveData.h"

#include "Ascend/UI/Player/PlayerInventory.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

#include "Engine/DamageEvents.h"


static FName NAME_ViewCamera(TEXT("ViewCamera"));
static FName Name_DataTransceiver(TEXT("DataTransceiver Capsule"));

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
#pragma region Character-Capsule

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

#pragma endregion
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
	PlayerCamera->SetupAttachment(GetMesh(), FName("spine_03"));

	// Set the relative location of the camera.
	FVector CameraRelativeLocation(25.0f, 17.0f, 0.0f);
	PlayerCamera->SetRelativeLocation(CameraRelativeLocation);

	// Set the relative rotation of the camera.
	FRotator CameraRelativeRotation(0.0f, 90.0f, -90.0f);
	PlayerCamera->SetRelativeRotation(CameraRelativeRotation);

	// Set the projection mode of the camera to perspective.
	PlayerCamera->SetProjectionMode(ECameraProjectionMode::Perspective);

	// Set the field of view of the camera.
	PlayerCamera->FieldOfView = 110.0f;

	// Lock the camera to the head-mounted display (HMD).
	PlayerCamera->bLockToHmd = true;

	// Use the field of view for level of detail (LOD) calculations.
	PlayerCamera->bUseFieldOfViewForLOD = true;
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

	CharacterMovementComp->MaxJumpApexAttemptsPerSimulation = 2;
	CharacterMovementComp->JumpZVelocity					= 505.0f;
	CharacterMovementComp->BrakingDecelerationFalling		= 0.001f;
	CharacterMovementComp->AirControl						= 0.07f;
	CharacterMovementComp->AirControlBoostMultiplier		= 1.30f;
	CharacterMovementComp->AirControlBoostVelocityThreshold = 20.0f;
	CharacterMovementComp->FallingLateralFriction			= 0.0f;
	CharacterMovementComp->bImpartBaseVelocityX				= true;
	CharacterMovementComp->bImpartBaseVelocityY				= true;
	CharacterMovementComp->bImpartBaseVelocityZ				= true;
	CharacterMovementComp->bNotifyApex						= false;
	CharacterMovementComp->JumpOffJumpZFactor				= 0.05f;
	CharacterMovementComp->bApplyGravityWhileJumping		= true;
	CharacterMovementComp->JumpOutOfWaterPitch				= 11.50f;

	// Control whether the character's rotation follows the controller's rotation for pitch and yaw.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw	= true;
	bUseControllerRotationRoll	= false;
}

void ABaseCharacter::CreateSubobjectComponents()
{
	static FName NAME_CombatSystem(TEXT("CombatSystem"));
	static FName NAME_ArsenalSystem(TEXT("ArsenalComponent"));
	static FName NAME_ViewInterface(TEXT("ViewInterfaceComponent"));
	static FName NAME_Attribute(TEXT("Attribute Component"));
	static FName NAME_LevelSystem(TEXT("LevelSystem Component"));


	CombatSystem		= CreateDefaultSubobject<UCombatSystemComponent>	(NAME_CombatSystem);
	ArsenalSystem		= CreateDefaultSubobject<UArsenalComponent>			(NAME_ArsenalSystem);
	ViewInterface		= CreateDefaultSubobject<UViewInterfaceComponent>	(NAME_ViewInterface);
	AttributeComponent	= CreateDefaultSubobject<UAttributeComponent>		(NAME_Attribute);
	LevelSystem			= CreateDefaultSubobject<ULevelSystemComponent>		(NAME_LevelSystem);
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

	if (LevelSystem)
	{
		LevelSystem->SetBaseCharacter(this);
	}
}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterAnimInstance = Cast<UCharacterDefaultAnimInstance>(GetMesh()->GetAnimInstance());
	if (CharacterAnimInstance)
	{
		CharacterAnimInstance->SetWeaponType(EWeaponType::EWT_Fists);
	}	
	
	PlayerStateModule = GetPlayerState<APlayerStateModule>();
	HUDInitialSetup();
	UpdateHUDExperience();
	ControlClassesAndComponentsCast();
	CreateInventoryWidget();

	//bool bSaveGame = UGameplayStatics::DoesSaveGameExist
	//(
	//	TEXT("SaveSlotName"),
	//	0
	//);

	//if (bSaveGame)
	//{
	//	UGameplayStatics::LoadGameFromSlot
	//	(
	//		TEXT("SaveSlotName"),
	//		0
	//	);
	//	UGameSaveData* LoadedSaveGameData = Cast<UGameSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("SaveSlotName"), 0));

	//	FTransform LoadedTransform = LoadedSaveGameData->PlayerTransform;
	//	SetActorTransform(LoadedTransform);
	//	LoadedSaveGameData->PlayerLevel = LevelSystem->GetLevelStructure().GetPlayerLevel();
	//	LoadedSaveGameData->TotalExperiencePoints = LevelSystem->GetLevelStructure().GetTotalExperience();

	//	SetActorTransform(LoadedSaveGameData->PlayerTransform);
	//}
	//else
	//{
	//	UGameSaveData* SaveGameData = Cast<UGameSaveData>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	//	if (SaveGameData)
	//	{
	//		// Assign your player's transform to the save game object
	//		SaveGameData->PlayerTransform		= GetActorTransform();
	//		SaveGameData->PlayerLevel			= LevelSystem->GetLevelStructure().GetPlayerLevel();
	//		SaveGameData->TotalExperiencePoints = LevelSystem->GetLevelStructure().GetTotalExperience();
	//		// Replace "YourSaveSlotName" with the name of your save slot
	//		UGameplayStatics::SaveGameToSlot(SaveGameData, TEXT("SaveSlotName"), 0); 
	//	}
	//}

	//UGameSaveData* LoadData = UGameSaveData::LoadGameData();
	//if (LoadData)
	//{
	//	int32 LoadLevel = LoadData->PlayerLevel;
	//	float LoadedExperience = LoadData->TotalExperiencePoints;
	//}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleAttributes();
	ResetLean();
}

void ABaseCharacter::ControlClassesAndComponentsCast()
{
#if CONTROL_AND_DEBUG_CLASSCOMPONENTS_CHECK == 1

	if (PlayerControllerModule)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerControllerModule successfully initialized!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerControllerModule is nullptr!"));
	}

	if (PlayerStateModule)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerStateModule successfully initialized!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerStateModule is nullptr!"));
	}

	if (ArsenalSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ArsenalSystem successfully initialized!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ArsenalSystem is nullptr!"));
	}

	if (PlayerCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCamera successfully initialized!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCamera is nullptr!"));
	}

	if (CharacterCapsule)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterCapsule successfully initialized!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterCapsule is nullptr!"));
	}

	if (CharacterMovementComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterMovementComp successfully initialized!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterMovementComp is nullptr!"));
	}

	if (CharacterAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterAnimInstance successfully initialized!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterAnimInstance is nullptr!"));
	}

	if (CombatSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatSystem successfully initialized!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CombatSystem is nullptr!"));
	}

	if (ViewInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("ViewInterface successfully initialized!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ViewInterface is nullptr!"));
	}

	if (AttributeComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttributeComponent successfully initialized!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeComponent is nullptr!"));
	}
#endif // CONTROL_AND_DEBUG_CLASSCOMPONENTS_CHECK
}
#pragma region Data-Receiver& Transmitter

void ABaseCharacter::DataTransceiverBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Leveling database
	if (LevelSystem != nullptr)
	{
		int32 PlayerLevel = LevelSystem->GetLevelStructure().GetPlayerLevel();

		if (OtherActor)
		{
			if (OtherActor->IsA<AAIEntityModule>())
			{
				AAIEntityModule* AIEntity = Cast<AAIEntityModule>(OtherActor);
				if (AIEntity)
				{

					AIEntity->GetAILevelSystemComponent()->GetLevelStructure().SetAIEntityLevel(PlayerLevel);
				}
			}
		}
	}
}

void ABaseCharacter::DataTransceiverEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (OtherActor)
		{
			if (OtherActor->IsA<AAIEntityModule>())
			{
				AAIEntityModule* AIEntity = Cast<AAIEntityModule>(OtherActor);

				if (AIEntity)
				{
					AIEntity = nullptr;
				}
			}
		}
	}
}
#pragma endregion

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
		EnhancedInputComponent->BindAction(MoveInputAction,				ETriggerEvent::Triggered,	this, &ABaseCharacter::InputFunctionMovement);
		EnhancedInputComponent->BindAction(MoveInputAction,				ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionMovementC);
		EnhancedInputComponent->BindAction(LookInputAction,				ETriggerEvent::Triggered,	this, &ABaseCharacter::InputFunctionLook);
		EnhancedInputComponent->BindAction(JumpInputAction,				ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionJump);
		EnhancedInputComponent->BindAction(CrouchInputAction,			ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionCrouch);
		EnhancedInputComponent->BindAction(CrouchInputAction,			ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionCrouch);
		EnhancedInputComponent->BindAction(ProneInputAction,			ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionProne);
		EnhancedInputComponent->BindAction(SprintInputAction,			ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionSprint);
		EnhancedInputComponent->BindAction(SprintInputAction,			ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionSprintEnd);

		EnhancedInputComponent->BindAction(AttackInputAction,			ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionAttack);
		EnhancedInputComponent->BindAction(AttackInputAction,			ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionAttackEnd);

		EnhancedInputComponent->BindAction(LeanRightInputAction,		ETriggerEvent::Triggered,	this, &ABaseCharacter::InputFunctionLean);
		EnhancedInputComponent->BindAction(LeanRightInputAction,		ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionLeanCompleted);

		EnhancedInputComponent->BindAction(AimInputAction,				ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionAim);
		EnhancedInputComponent->BindAction(AimInputAction,				ETriggerEvent::Completed,	this, &ABaseCharacter::InputFunctionAimEnd);

		EnhancedInputComponent->BindAction(InteractionInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionInteraction);

		EnhancedInputComponent->BindAction(WeaponSwitchInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionSwapWeapon);

		EnhancedInputComponent->BindAction(FireModeSwitchInputAction,	ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionWeaponFiringModeSwitch);
		EnhancedInputComponent->BindAction(FireTypeSwitchInputAction,	ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionWeaponFireTypeSwitch);

		EnhancedInputComponent->BindAction(InventoryInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::InputFunctionInventoryMenu);

		EnhancedInputComponent->BindAction(ReloadInputAction,			ETriggerEvent::Started,     this, &ABaseCharacter::InputFunctionReload);
		
		EnhancedInputComponent->BindAction(DebugInputAction,			ETriggerEvent::Started,		this, &ABaseCharacter::DebugInput);
	}
}

void ABaseCharacter::DebugInput()
{

}

/*Dynamics*/
void ABaseCharacter::InputFunctionMovement(const FInputActionValue& Value)
{
	if (Controller == nullptr || PlayerIsDead()) 	return;

	FVector2D MovementValue = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MovementValue.Y);
	AddMovementInput(GetActorRightVector(), MovementValue.X);

	CharacterForwardDirection = MovementValue.Y;
	CharacterRightDirection   = MovementValue.X;

	UpdateSpeedAndDirections();
}

void ABaseCharacter::InputFunctionMovementC(const FInputActionValue& Value)
{
	if (Controller == nullptr || PlayerIsDead())
		return;

	FVector2D MovementValue = Value.Get<FVector2D>();

	CharacterForwardDirection = MovementValue.Y;
	CharacterRightDirection   = MovementValue.X;

	ResetSpeedAndDirections();
}

void ABaseCharacter::ControlCharacterMovementSpeed()
{
	if (PlayerIsDead()) return;

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
	if (Controller == nullptr || PlayerIsDead() || bIsInInventoryWidget == true)
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
	if (Controller == nullptr || PlayerIsDead()) return;

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
	if (Controller == nullptr || PlayerIsDead()) return;

	if (bWantsToLean != false)
	{
		bWantsToLean = false;
		bCharacterLeaning = bWantsToLean;
	}
}

void ABaseCharacter::ResetLean()
{
	if (Controller == nullptr || PlayerIsDead()) return;

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
	if (Controller == nullptr || PlayerIsDead()) return;

	if (bCharacterCrouching == true || bCharacterProne == true ) return;
	bPressedJump = true;
	Super::Jump();
}

void ABaseCharacter::InputFunctionCrouch()
{
	if (Controller == nullptr || PlayerIsDead()) return;

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
	if (Controller == nullptr || PlayerIsDead()) return;

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
	}
	else
	{
		if (bCharacterProne == true) bCharacterProne = false;
		UnCrouch();
		ControlCharacterMovementSpeed();
	}
}

void ABaseCharacter::InputFunctionSprint()
{
	if (Controller == nullptr || PlayerIsDead()) return;

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
	if (Controller == nullptr || PlayerIsDead()) return;

	if (bAttackButtonPressed || bAimButtonPressed) return;

	bWantsToSprint = false;
	bCharacterSprinting = bWantsToSprint;
	ControlCharacterMovementSpeed();
}

void ABaseCharacter::InputFunctionInteraction()
{
	if (Controller == nullptr || PlayerIsDead()) return;

	if (OverlapWeapon != nullptr)
	{
		ArsenalSystem->SelectWeaponTypes(OverlapWeapon);
	}
}

void ABaseCharacter::InputFunctionAttack()
{
	if (Controller == nullptr || PlayerIsDead()) return;

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
	if (Controller == nullptr || PlayerIsDead()) return;

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
	if (Controller == nullptr || PlayerIsDead()) return;

	if (CombatSystem && CombatSystem->CanAim())
	{
		bAimButtonPressed = true;
		CharacterAnimInstance->SetIsAiming(bAimButtonPressed);
		bCharacterAiming  = bAimButtonPressed;
	}
	ControlCharacterMovementSpeed();
}

void ABaseCharacter::InputFunctionAimEnd()
{
	if (Controller == nullptr || PlayerIsDead()) return;

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
	if (Controller == nullptr || PlayerIsDead()) return;

	if (!ArsenalSystem)
	{
		return;
	}

	if (!ArsenalSystem->GetPrimaryEquipWeapon())
	{
		return;
	}
		
	SwitchWeaponFiringMode(ArsenalSystem->GetRangedWeapon());
}

void ABaseCharacter::SwitchWeaponFiringMode(ARangedWeapon* InputWeapon)
{
	if (Controller == nullptr || PlayerIsDead()) return;

	if (InputWeapon == nullptr) return;

	if (!(InputWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Rifle ||
		InputWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Heavymachine ||
		InputWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Submachine))
	{
		return;
	}

	if (InputWeapon->GetRangedWeaponProperties().GetWeaponFireMode() == EWeaponFiringMode::SingleShot)
	{
		InputWeapon->SetWeaponFireMode(EWeaponFiringMode::SemiAutomatic);
	}
	else if (InputWeapon->GetRangedWeaponProperties().GetWeaponFireMode() == EWeaponFiringMode::SemiAutomatic)
	{
		InputWeapon->SetWeaponFireMode(EWeaponFiringMode::Automatic);
	}
	else if (InputWeapon->GetRangedWeaponProperties().GetWeaponFireMode() == EWeaponFiringMode::Automatic)
	{
		InputWeapon->SetWeaponFireMode(EWeaponFiringMode::SingleShot);
	}

}

void ABaseCharacter::InputFunctionWeaponFireTypeSwitch()
{
	if (Controller == nullptr || PlayerIsDead()) return;

	if (!ArsenalSystem)
	{
		return;
	}

	if (!ArsenalSystem->GetPrimaryEquipWeapon())
	{
		return;
	}


	SwitchWeaponFireType(ArsenalSystem->GetRangedWeapon());
}

void ABaseCharacter::SwitchWeaponFireType(ARangedWeapon* InputWeapon)
{
	if (Controller == nullptr || PlayerIsDead()) return;

	if (InputWeapon == nullptr) return;

	if (!(InputWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Rifle ||
		InputWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Heavymachine ||
		InputWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Submachine ||
		InputWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Crossbow ||
		InputWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Pistol ||
		InputWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Shotgun ||
		InputWeapon->GetRangedWeaponProperties().GetRangedWeaponType() == ERangedWeaponType::ERT_Sniper))
	{
		return;
	}

	if (InputWeapon->GetRangedWeaponProperties().GetFireType() == EFireType::EFT_HitScan)
	{
		InputWeapon->SetWeaponFireType(EFireType::EFT_Projectile);
	}
	else if (InputWeapon->GetRangedWeaponProperties().GetFireType() == EFireType::EFT_Projectile)
	{
		InputWeapon->SetWeaponFireType(EFireType::EFT_HitScan);
	}
}

void ABaseCharacter::InputFunctionEnterVehicle()
{
	if (Controller == nullptr || PlayerIsDead()) return;

}

void ABaseCharacter::InputFunctionSwapWeapon()
{
	if (Controller == nullptr || PlayerIsDead() || ArsenalSystem == nullptr) return;

	if (ArsenalSystem->GetPrimaryEquipWeapon() == nullptr) return;
}
void ABaseCharacter::InputFunctionReload()
{
	if (Controller == nullptr || PlayerIsDead()) return;

	if (CombatSystem == nullptr && ArsenalSystem == nullptr ) return;

	if (ArsenalSystem->GetPrimaryEquipWeapon() == nullptr) return;
	CombatSystem->Reload();
	bWantsToReload = true;
}

void ABaseCharacter::InputFunctionSlide()
{
	if (Controller == nullptr || PlayerIsDead()) return;

}

void ABaseCharacter::InputFunctionDash()
{
	if (Controller == nullptr || PlayerIsDead()) return;

}

void ABaseCharacter::InputFunctionOption()
{
	if (Controller == nullptr || PlayerIsDead()) return;

}

void ABaseCharacter::InputFunctionInventoryMenu()
{
	bInventory = !bInventory;


	// Change game speed based on bInventory
	if (GetWorld() != nullptr && GetWorld()->GetWorldSettings() != nullptr)
	{
		AWorldSettings* WorldSettings = GetWorld()->GetWorldSettings();

		if (bInventory)
		{
			// Set game speed to slow motion (0.1)
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01f);
		}
		else
		{
			// Set game speed to normal (1.0)
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}
	}
	

	if (PlayerInventoryWidget != nullptr)
	{
		if (bInventory)
		{
			if (PlayerControllerModule != nullptr)
			{
				PlayerControllerModule->bShowMouseCursor = true;

				FInputModeGameAndUI InputMode;
				InputMode.SetWidgetToFocus(PlayerInventoryWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				InputMode.SetHideCursorDuringCapture(true);
				PlayerControllerModule->SetInputMode(InputMode);
			}
			PlayerInventoryWidget->SetVisibility(ESlateVisibility::Visible);
			bIsInInventoryWidget = true;
		}
		else
		{
			if (PlayerControllerModule != nullptr)
			{
				PlayerControllerModule->bShowMouseCursor = false;

				FInputModeGameOnly  InputMode;
				PlayerControllerModule->SetInputMode(InputMode);
			}
			PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			bIsInInventoryWidget = false;
		}
	}
}


void ABaseCharacter::InputFunctionScore()
{
	if (Controller == nullptr || PlayerIsDead()) return;

}

bool ABaseCharacter::DisableAim()
{
	if (Controller == nullptr || PlayerIsDead()) return false;

	if (ArsenalSystem)
	{
		if (ArsenalSystem->GetPrimaryEquipWeapon() != nullptr)
		{
			if (ArsenalSystem->GetPrimaryEquipWeapon()->GetWeaponType() == EWeaponType::EWT_Melee)
			{
				return true;
			}
		}
	}
	return false;
}
#pragma endregion

#pragma region Movement
void ABaseCharacter::UpdateSpeedAndDirections()
{
	if (Controller == nullptr || PlayerIsDead()) return;

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
	if (Controller == nullptr || PlayerIsDead()) return;

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
	if (PlayerIsDead()) return 0.0f;
	DamageAmount = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return DamageAmount;
}

void ABaseCharacter::ReceiveDamage(float Damage)
{
	if (PlayerIsDead()) return;

	// Create a default damage event
	FDamageEvent DamageEvent;

	// Get the instigator controller
	AController* InstigatorController = GetInstigatorController();

	if (AttributeComponent != nullptr)
	{
		if (AttributeComponent->GetPlayerHealth() <= 0) return;

		float CurrentMaxHealth = AttributeComponent->GetPlayerMaxHealth();
		float CurrentHealth = AttributeComponent->GetPlayerHealth();
		float NewHealth = CurrentHealth - Damage;

		NewHealth = FMath::Clamp(NewHealth, 0.0f, CurrentMaxHealth);

		AttributeComponent->SetPlayerHealth(NewHealth);
		if (CurrentHealth <= 0)
		{
			Death();
		}
		else
		{
			if (PlayerControllerModule)
			{
				PlayerControllerModule->SetHUDHealth(NewHealth, CurrentMaxHealth,true);
			}
			else
			{
			}
		}
	}


	// Call TakeDamage with the provided damage value and default damage event
	TakeDamage(Damage, DamageEvent, InstigatorController, this);

	if (AttributeComponent->GetPlayerHealth() <= 0)
	{
		Death();
	}
}
#pragma endregion

#pragma region Death-State
void ABaseCharacter::Death()
{
	SetRagDoll(true, ECollisionEnabled::PhysicsOnly);
	bPlayerIsDead = true;

	GetWorldTimerManager().SetTimer
	(
		RespawnTimerHandler,
		this,
		&ABaseCharacter::RespawnPlayer,
		5.0f
	);
}

void ABaseCharacter::SetRagDoll(bool SimulatePhysics, ECollisionEnabled::Type NewType)
{
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	GetHairMesh()	->SetSimulatePhysics (SimulatePhysics);
	GetHeadMesh()	->SetSimulatePhysics (SimulatePhysics);
	GetMesh()		->SetSimulatePhysics (SimulatePhysics);
	GetLegsMesh()	->SetSimulatePhysics (SimulatePhysics);
	GetArmsMesh()	->SetSimulatePhysics (SimulatePhysics);
	GetHandsMesh()	->SetSimulatePhysics (SimulatePhysics);
	GetHairMesh()	->SetCollisionEnabled(NewType);
	GetHeadMesh()	->SetCollisionEnabled(NewType);
	GetMesh()		->SetCollisionEnabled(NewType);
	GetLegsMesh()	->SetCollisionEnabled(NewType);
	GetArmsMesh()	->SetCollisionEnabled(NewType);
	GetHandsMesh()	->SetCollisionEnabled(NewType);

	GetHairMesh()	->SetVisibility(false);
	GetHeadMesh()	->SetVisibility(false);
}

void ABaseCharacter::RespawnPlayer()
{
	ABaseGameMode* BaseGameMode  = GetWorld()->GetAuthGameMode<ABaseGameMode>();

	if (BaseGameMode)
	{
		AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

		if (PlayerStart)
		{
			// Restart the player at the player start location
			BaseGameMode->RestartPlayerAtPlayerStart(Controller, PlayerStart);
			LW("RespawnPlayer");
		}
		else
		{
			// Log an error if player start actor is not found
		}
	}
	else
	{
	}
}

#pragma endregion
#pragma region HUD

void ABaseCharacter::HUDInitialSetup()
{
	PlayerControllerModule = PlayerControllerModule == nullptr ? Cast<APlayerControllerModule>(Controller) : PlayerControllerModule;

	if (AttributeComponent != nullptr && PlayerControllerModule != nullptr)
	{
		InitializeHUDHealth();
		InitializeHUDShield();
		InitializeHUDStamina();
	}
	else
	{
	}
}

void ABaseCharacter::InitializeHUDHealth()
{
	if (PlayerIsDead()) return;

	// Get initial player's health 
	float PlayerHealth		= AttributeComponent->GetPlayerHealth();
	// Get initial player's max-health
	float PlayerMaxHealth	= AttributeComponent->GetPlayerMaxHealth();

	// Set player's current health and max-health to HUD 
	PlayerControllerModule->SetHUDHealth(PlayerHealth, PlayerMaxHealth, true);
}

void ABaseCharacter::InitializeHUDShield()
{
	if (PlayerIsDead()) return;

	// Get initial player's shield
	float PlayerShield = AttributeComponent->GetPlayerShield();

	// Get initial player's max-shield
	float PlayerMaxShield = AttributeComponent->GetPlayerMaxShield();

	// Set player's current shield and max-shield to HUD
	PlayerControllerModule->SetHUDShield(PlayerShield, PlayerMaxShield, true);
}

void ABaseCharacter::InitializeHUDStamina()
{
	if (PlayerIsDead()) return;

	// Get initial player's stamina amount
	float PlayerStamina = AttributeComponent->GetPlayerStamina();

	// Get initial player's max-stamina amount
	float PlayerMaxStamina = AttributeComponent->GetPlayerMaxStamina();

	// Set player's current stamina and max-stamina amount to HUD
	PlayerControllerModule->SetHUDStamina(PlayerStamina, PlayerMaxStamina, true);
}
#pragma endregion
#pragma region UI/UX/Widget
void ABaseCharacter::CreateInventoryWidget()
{
	if (InventoryWidget)
	{
		PlayerInventoryWidget = CreateWidget<UPlayerInventory>
		(
			GetWorld(),
			InventoryWidget
		);

		if (PlayerInventoryWidget)
		{
			PlayerInventoryWidget->AddToViewport();
			PlayerInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
#pragma endregion

#pragma region Attributes
void ABaseCharacter::HandleAttributes()
{
	HandleHealth();
	HandleShield();
	HandleStamina();
}
void ABaseCharacter::HandleHealth()
{
	if (PlayerIsDead()) return;

}

void ABaseCharacter::HandleShield()
{
	if (PlayerIsDead()) return;

}

void ABaseCharacter::HandleStamina()
{
	if (PlayerIsDead()) return;

	if (AttributeComponent)
	{
		if (bCharacterSprinting && GetVelocity().Size() > 0)
		{
			AttributeComponent->DecreaseStamina();

			if (PlayerControllerModule != nullptr)
			{
				// Get current player's stamina
				float CurrentPlayerStamina = AttributeComponent->GetPlayerStamina();
				// Get current player's max stamina 
				float CurrentPlayerMaxStamina = AttributeComponent->GetPlayerMaxStamina();

				// Set the HUD's stamina display to reflect the current player's stamina and maximum stamina
				PlayerControllerModule->SetHUDStamina(CurrentPlayerStamina, CurrentPlayerMaxStamina, true);

				if (CurrentPlayerStamina <= 0)
				{
					InputFunctionSprintEnd();
				}
			}
		}
		else
		{
			AttributeComponent->IncreaseStamina();
			if (PlayerControllerModule != nullptr)
			{
				// Get current player's stamina
				float CurrentPlayerStamina = AttributeComponent->GetPlayerStamina();
				// Get current player's max stamina 
				float CurrentPlayerMaxStamina = AttributeComponent->GetPlayerMaxStamina();

				// Set the HUD's stamina display to reflect the current player's stamina and maximum stamina
				PlayerControllerModule->SetHUDStamina(CurrentPlayerStamina, CurrentPlayerMaxStamina, true);
			}
		}
	}
}

void ABaseCharacter::UpdateHUDExperience()
{
	if (PlayerControllerModule != nullptr)
	{
		float CurrentTotalExperience = LevelSystem->GetLevelStructure().GetTotalExperience();
		float CurrentLevelExperienceCap = LevelSystem->GetCurrentLevelThreshold();
		bool bIsAlive = AttributeComponent->PlayerAlive();
		uint32 PlayerLevel = LevelSystem->GetLevelStructure().GetPlayerLevel();
		LevelSystem->SaveExperienceAndLevel
		(
			PlayerLevel,
			CurrentTotalExperience
		);

		
		PlayerControllerModule->SetHUDExperience(CurrentTotalExperience, CurrentLevelExperienceCap, PlayerLevel, bIsAlive);
	}
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

#pragma region Reloading
void ABaseCharacter::PlayReloadMontage()
{
	if (PlayerIsDead()) return;

	bool ValidConfiguration =
		ReloadMontage							!= nullptr	||
		CombatSystem							!= nullptr	||
		ArsenalSystem							!= nullptr	||
		ArsenalSystem->GetRangedWeapon()		!= nullptr	||
		ArsenalSystem->GetPrimaryEquipWeapon()	!= nullptr;

	if (ValidConfiguration == false)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;

		switch (ArsenalSystem->GetRangeWeaponType())
		{
		case ERangedWeaponType::ERT_Pistol:
			SectionName = FName("Pistol");
			break;

		case ERangedWeaponType::ERT_Rifle:
			SectionName = FName("Rifle");
			break;
		case ERangedWeaponType::ERT_Submachine:
			break;

		case ERangedWeaponType::ERT_Heavymachine:
			break;

		case ERangedWeaponType::ERT_Shotgun:
			break;

		case ERangedWeaponType::ERT_Sniper:
			break;

		case ERangedWeaponType::ERT_Bow:
			break;

		case ERangedWeaponType::ERT_Launcher:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}
void ABaseCharacter::SetReloading(bool bReload)
{
	if (PlayerIsDead()) return;

	if (CombatSystem)
	{
		CombatSystem->SetReloading(bReload);
	}
}
#pragma endregion
