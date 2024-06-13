// @2023 All rights reversed by Reverse-Alpha Studios


#include "Characters/BaseCharacter.h"

#include "SyntheticA/SyntheticA.h"

#include "Aeronautical/Aerodyne.h"
#include "Animation/CharacterAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraManager.h"
#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "Components/EnhancedMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/LagCompensationComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/BaseGameMode.h"
#include "GameModes/CaptureTheFlagGameMode.h"
#include "GameModes/CaptureTheFlag/Objects/Flag.h"
#include "HUD/GameOption.h"
#include "HUD/Scoreboard.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerController/PlayerControllerManager.h"
#include "PlayerStart/TeamPlayerStart.h"
#include "PlayerState/PlayerStateManager.h"
#include "Pickups/Pickup.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "Vehicle/GroundVehiclePawn.h"

#include "Weapon/Weapon.h"

#include "Net/UnrealNetwork.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

#pragma region General
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UEnhancedMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Enables ticking for the actor.
	PrimaryActorTick.bCanEverTick	= true;

	// Adjusts collision handling method during spawning.
	SpawnCollisionHandlingMethod	= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Configures networking parameters such as loading on client, relevancy, dormancy, update frequency, and priority.
	bNetLoadOnClient				= true;
	bNetUseOwnerRelevancy			= false;
	NetDormancy						= DORM_Awake;
	NetCullDistanceSquared			= 900000000.0f;
	NetUpdateFrequency				= 66.0f;
	MinNetUpdateFrequency			= 33.0f;
	NetPriority						= 3.0f;

	// Initializes the CapsuleComponent with a specified size.
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);

	// Ignores collisions with the camera
	CapsuleComp						->InitCapsuleSize(35.0f, 89.95f);
	CapsuleComp						->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()						->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Retrieves the character's skeletal mesh component
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	// Sets up attachment to the CharacterMesh with a specific relative location and rotation
	FVector  MeshRelativeLocation	= FVector (0.0f, 0.0f, -90.0f);
	FRotator MeshRelativeRotation	= FRotator(0.0f, 0.0f, -90.0f);

	// Give info
	CharacterMesh					->SetupAttachment	 (CapsuleComp);
	CharacterMesh					->SetRelativeLocation(MeshRelativeLocation);
	CharacterMesh					->SetRelativeRotation(MeshRelativeRotation);

	// Creates Sub-objects for different parts of the character (Hair, Head, Legs, Arms, Hands).
	Hair							= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Head							= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));

	Legs							= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Arms							= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Hands							= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));

	// Sets collision object type for each skeletal mesh component to ECC_SkeletalMesh.
	CharacterMesh					->SetCollisionObjectType(ECC_SkeletalMesh);
	Hair							->SetCollisionObjectType(ECC_SkeletalMesh);
	Head							->SetCollisionObjectType(ECC_SkeletalMesh);
	Legs							->SetCollisionObjectType(ECC_SkeletalMesh);
	Arms							->SetCollisionObjectType(ECC_SkeletalMesh);
	Hands							->SetCollisionObjectType(ECC_SkeletalMesh);

	//Sets collision response for each skeletal mesh component to ECR_Block for the visibility channel.
	CharacterMesh					->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Hair							->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Head							->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Legs							->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Arms							->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Hands							->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	//Sets up attachment relationships for Sub-objects (Head, Hair, Legs, Arms, Hands) to the main character mesh (CharacterMesh).
	Head							->SetupAttachment(CharacterMesh);
	Hair							->SetupAttachment(CharacterMesh);
	Legs							->SetupAttachment(CharacterMesh);
	Arms							->SetupAttachment(CharacterMesh);
	Hands							->SetupAttachment(CharacterMesh);

	/* Camera Props*/
	CameraManager					= CreateDefaultSubobject<UCameraManager>		(TEXT("CameraManager"));

	DeathCamera						= CreateDefaultSubobject<UCameraComponent>		(TEXT("DeathCamera"));
	DeathCamera						->SetupAttachment								(CapsuleComp);
	DeathCamera						->SetRelativeLocation							(FVector(0.0f, 0.0f, 500.0f));
	DeathCamera						->SetRelativeRotation							(FRotator(0.0f, -90.0f, 0.0f));

	ThirdPersonSpringArm			= CreateDefaultSubobject<USpringArmComponent>	(TEXT("TPPSpringArm"));
	FirstPersonCamera				= CreateDefaultSubobject<UCameraComponent>		(TEXT("FirstPersonCamera"));
	ThirdPersonCamera				= CreateDefaultSubobject<UCameraComponent>		(TEXT("ViewCamera"));

	ThirdPersonSpringArm			->SetupAttachment			(CharacterMesh);
	ThirdPersonSpringArm			->SetRelativeLocation		(FVector(0.0f, 0.0f, 160.0f));
	ThirdPersonSpringArm			->TargetArmLength			= 300.0f;
	ThirdPersonSpringArm			->SocketOffset				= FVector(0.0f, 65.0f, 30.0f);
	ThirdPersonSpringArm			->bUsePawnControlRotation	= true;

	ThirdPersonCamera				->SetupAttachment			(ThirdPersonSpringArm);
	ThirdPersonCamera				->FieldOfView				= 80.0f;
	ThirdPersonCamera				->bUsePawnControlRotation	= false;

	ThirdPersonCamera				->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	ThirdPersonCamera				->PostProcessSettings.DepthOfFieldFocalDistance			  = 10000.f;
	ThirdPersonCamera				->PostProcessSettings.bOverride_DepthOfFieldFstop		  = true;
	ThirdPersonCamera				->PostProcessSettings.DepthOfFieldFstop					  = 32.0f;

	FirstPersonCamera				->SetupAttachment(Head);
	FirstPersonCamera				->bUsePawnControlRotation = true;
	FirstPersonCamera				->SetWorldLocation(FVector(0.0f, 20.0f, 0.0f));
	FirstPersonCamera				->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	FirstPersonCamera				->bAutoActivate = false;

	bUseControllerRotationPitch		= false;
	bUseControllerRotationYaw		= false;
	bUseControllerRotationRoll		= false;
	BaseEyeHeight					= 80.0f;
	CrouchedEyeHeight				= 50.0f;

	AttributeComponent				= CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	AttributeComponent				->SetIsReplicated(true);

	CombatComponent					= CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent					->SetIsReplicated(true);

	EnhancedMovementComponent		= Cast<UEnhancedMovementComponent>(GetCharacterMovement());
	EnhancedMovementComponent		->bOrientRotationToMovement = true;
	EnhancedMovementComponent		->SetIsReplicated(true);

	LagCompensantionComponent		= CreateDefaultSubobject<ULagCompensationComponent>(TEXT("LagCompensationComponent"));

	SpawnCollisionHandlingMethod	= ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	DissolveTimeline				= CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent-Dissolve"));
#pragma region Hit-boxes
	Head_Box						= CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBox"));
	Head_Box						->SetupAttachment(GetHeadMesh(), FName("head"));
	HitCollisionBoxes.Add			(FName("head"), Head_Box);

	Pelvis_Box						= CreateDefaultSubobject<UBoxComponent>(TEXT("PelvisBox"));
	Pelvis_Box						->SetupAttachment(GetMesh(), FName("pelvis"));
	HitCollisionBoxes.Add			(FName("pelvis"), Pelvis_Box);

	Spine_02_Box					= CreateDefaultSubobject<UBoxComponent>(TEXT("Spine_02Box"));
	Spine_02_Box					->SetupAttachment(GetMesh(), FName("spine_02"));
	HitCollisionBoxes.Add			(FName("spine_02"), Spine_02_Box);

	Spine_03_Box					= CreateDefaultSubobject<UBoxComponent>(TEXT("Spine_03Box"));
	Spine_03_Box					->SetupAttachment(GetMesh(), FName("spine_03"));
	HitCollisionBoxes.Add			(FName("spine_03"), Spine_03_Box);

	UpperArm_R_Box					= CreateDefaultSubobject<UBoxComponent>(TEXT("UpperArm_R_Box"));
	UpperArm_R_Box					->SetupAttachment(GetArmsMesh(), FName("upperarm_r"));
	HitCollisionBoxes.Add			(FName("upperarm_r"), UpperArm_R_Box);

	UpperArm_L_Box					= CreateDefaultSubobject<UBoxComponent>(TEXT("UpperArm_L_Box"));
	UpperArm_L_Box					->SetupAttachment(GetArmsMesh(), FName("upperarm_l"));
	HitCollisionBoxes.Add			(FName("upperarm_l"), UpperArm_L_Box);

	LowerArm_R_Box					= CreateDefaultSubobject<UBoxComponent>(TEXT("LowerArm_R_Box"));
	LowerArm_R_Box					->SetupAttachment(GetArmsMesh(), FName("lowerarm_r"));
	HitCollisionBoxes.Add			(FName("lowerarm_r"), LowerArm_R_Box);

	LowerArm_L_Box					= CreateDefaultSubobject<UBoxComponent>(TEXT("LowerArm_L_Box"));
	LowerArm_L_Box					->SetupAttachment(GetArmsMesh(), FName("lowerarm_l"));
	LowerArm_L_Box					->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitCollisionBoxes.Add			(FName("lowerarm_l"), LowerArm_L_Box);

	Hand_R_Box						= CreateDefaultSubobject<UBoxComponent>(TEXT("Hand_R_Box"));
	Hand_R_Box						->SetupAttachment(GetHandsMesh(), FName("hand_r"));
	HitCollisionBoxes.Add			(FName("hand_r"), Hand_R_Box);

	Hand_L_Box						= CreateDefaultSubobject<UBoxComponent>(TEXT("Hand_L_Box"));
	Hand_L_Box						->SetupAttachment(GetHandsMesh(), FName("hand_l"));
	HitCollisionBoxes.Add			(FName("hand_l"), Hand_L_Box);

	Thigh_R_Box						 = CreateDefaultSubobject<UBoxComponent>(TEXT("Thigh_R_Box"));
	Thigh_R_Box						->SetupAttachment(GetLegsMesh(), FName("thigh_r"));
	HitCollisionBoxes.Add			(FName("thigh_r"), Thigh_R_Box);

	Thigh_L_Box						= CreateDefaultSubobject<UBoxComponent>(TEXT("Thigh_L_Box"));
	Thigh_L_Box						->SetupAttachment(GetLegsMesh(), FName("thigh_l"));
	HitCollisionBoxes.Add			(FName("thigh_l"), Thigh_L_Box);

	Calf_R_Box						= CreateDefaultSubobject<UBoxComponent>(TEXT("Calf_R_Box"));
	Calf_R_Box						->SetupAttachment(GetLegsMesh(), FName("calf_r"));
	HitCollisionBoxes.Add			(FName("calf_r"), Calf_R_Box);

	Calf_L_Box						= CreateDefaultSubobject<UBoxComponent>(TEXT("Calf_L_Box"));
	Calf_L_Box						->SetupAttachment(GetLegsMesh(), FName("calf_l"));
	HitCollisionBoxes.Add			(FName("calf_l"), Calf_L_Box);

	Foot_R_Box						= CreateDefaultSubobject<UBoxComponent>(TEXT("Foot_R_Box"));
	Foot_R_Box						->SetupAttachment(GetLegsMesh(), FName("foot_r"));
	HitCollisionBoxes.Add			(FName("foot_r"), Foot_R_Box);

	Foot_L_Box						= CreateDefaultSubobject<UBoxComponent>(TEXT("Foot_L_Box"));
	Foot_L_Box						->SetupAttachment(GetLegsMesh(), FName("foot_l"));
	HitCollisionBoxes.Add			(FName("foot_l"), Foot_L_Box);

	BackPack_Box					= CreateDefaultSubobject<UBoxComponent>(TEXT("BackPack_Box"));
	BackPack_Box					->SetupAttachment(GetMesh(), FName("spine_03"));
	HitCollisionBoxes.Add			(FName("back_pack"), BackPack_Box);

	for (auto Box : HitCollisionBoxes)
	{
		if (Box.Value)
		{
			Box.Value->SetCollisionObjectType(ECC_HitBox);
			Box.Value->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			Box.Value->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);
			Box.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
#pragma endregion
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent)
	{
		CombatComponent->BaseCharacter = this;
	}

	if (AttributeComponent)
	{
		AttributeComponent->BaseCharacter = this;
		AttributeComponent->SetInitialSpeed(GetCharacterMovement()->MaxWalkSpeed, GetCharacterMovement()->MaxWalkSpeedCrouched);
		AttributeComponent->SetInitialJumpVelocity(GetCharacterMovement()->JumpZVelocity);
	}

	if (LagCompensantionComponent)
	{
		LagCompensantionComponent->Character = this;
		if (Controller)
		{
			LagCompensantionComponent->ControllerManager = Cast<APlayerControllerManager>(Controller);
		}
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	EnhancedInputLocalPlayerSubsystem();
	GameModeClassName = GetGameModeClassName();

	EnhancedMovementComponent = EnhancedMovementComponent	== nullptr ? Cast<UEnhancedMovementComponent>(GetCharacterMovement()) : EnhancedMovementComponent;
	CameraManager			  = CameraManager				== nullptr ? Cast<UCameraManager>(CameraManager): CameraManager;
	PlayerControllerManager	  = PlayerControllerManager		== nullptr ? Cast<APlayerControllerManager>(Controller)	: PlayerControllerManager;
	PlayerStateManager		  = PlayerStateManager			== nullptr ? GetPlayerState<APlayerStateManager>()	: PlayerStateManager;
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABaseCharacter::ReceiveDamage);
	}

	if (DeathCamera->IsActive())
		DeathCamera->SetActive(false);

	CreateInGameOptionMenu();
	CreateScoreboardWidget();
	SpawnDefaultWeapon();
	UpdateHUDHealth(false);
	UpdateHUDShield(false);
	UpdateHUDAmmunition();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckOnPlayerStateInitialized();
	DeathCondition(DeltaTime);
	//if (GetPlayerInVehicle())
	//{
	//	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(Controller) : PlayerControllerManager;
	//	if (PlayerControllerManager)
	//	{
	//		PlayerControllerManager->SetHUDIfPlayerInVehicle(true);
	//	}
	//}
	//else
	//{
	//	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(Controller) : PlayerControllerManager;
	//	if (PlayerControllerManager)
	//	{
	//		PlayerControllerManager->SetHUDIfPlayerInVehicle(true);
	//	}
	//}

	if (PlayerControllerManager == nullptr)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(Controller) : PlayerControllerManager;
		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDCarriedAmmo	(CombatComponent->CarriedAmmo);
			PlayerControllerManager->SetHUDWeaponAmmo	(CombatComponent->EquippedWeapon->Get_Ammo());
			PlayerControllerManager->SetHUDShield		(Shield, MaxShield);
			PlayerControllerManager->SetHUDHealth		(Health, MaxHealth);
		}
	}	
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(CameraSwitchAction,		ETriggerEvent::Started,		this, &ABaseCharacter::Input_SwitchCamera);
		EnhancedInputComponent->BindAction(MoveInputAction,			ETriggerEvent::Triggered,	this, &ABaseCharacter::Input_Movement);
		EnhancedInputComponent->BindAction(LookInputAction,			ETriggerEvent::Triggered,	this, &ABaseCharacter::Input_Look);

		EnhancedInputComponent->BindAction(SprintInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::Input_Sprint);
		EnhancedInputComponent->BindAction(JumpInputAction,			ETriggerEvent::Started,		this, &ABaseCharacter::Input_Jump);

		EnhancedInputComponent->BindAction(FireInputAction,			ETriggerEvent::Started,		this, &ABaseCharacter::Input_Fire_Activation);
		EnhancedInputComponent->BindAction(FireInputAction,			ETriggerEvent::Completed,	this, &ABaseCharacter::Input_Fire_DeActivation);

		EnhancedInputComponent->BindAction(CrouchInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::Input_Crouch_Activation);
		EnhancedInputComponent->BindAction(CrouchInputAction,		ETriggerEvent::Completed,	this, &ABaseCharacter::Input_Crouch_DeActivation);

		EnhancedInputComponent->BindAction(InteractionInputAction,  ETriggerEvent::Started,		this, &ABaseCharacter::Input_Interaction);
		EnhancedInputComponent->BindAction(EnterVehicleInputAction, ETriggerEvent::Started,		this, &ABaseCharacter::Input_EnterVehicleAction);
		EnhancedInputComponent->BindAction(SwapWeaponInputAction,	ETriggerEvent::Started,		this, &ABaseCharacter::Input_SwapWeapon);
		EnhancedInputComponent->BindAction(ReloadInputAction,		ETriggerEvent::Started,		this, &ABaseCharacter::Input_Reload);

		EnhancedInputComponent->BindAction(AimInputAction,			ETriggerEvent::Started,		this, &ABaseCharacter::Input_AimButton_Activation);
		EnhancedInputComponent->BindAction(AimInputAction,			ETriggerEvent::Completed,	this, &ABaseCharacter::Input_AimButton_DeActivation);

		EnhancedInputComponent->BindAction(InGameOptionInputAction, ETriggerEvent::Started,		this, &ABaseCharacter::InputAction_InGameOptionMenu);

		EnhancedInputComponent->BindAction(ScoreboardInputAction,	ETriggerEvent::Started,		this, &ABaseCharacter::InputAction_Scoreboard);
	}
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BaseCharacterMappingContext, 0);
		}
	}
}

void ABaseCharacter::Destroyed()
{
	bool bMatchNotInProgress = BaseGameMode && BaseGameMode->GetMatchState() != MatchState::InProgress;
	if (CombatComponent && CombatComponent->EquippedWeapon && bMatchNotInProgress)
	{
		CombatComponent->EquippedWeapon->Destroy();
	}
}
#pragma region Networking
void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABaseCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ABaseCharacter, OverlappingFlag, COND_OwnerOnly);
	DOREPLIFETIME(ABaseCharacter, AerodyneClass);
	DOREPLIFETIME_CONDITION(ABaseCharacter, GroundVehiclePawn, COND_OwnerOnly);

	DOREPLIFETIME(ABaseCharacter, MovementDirectionX);
	DOREPLIFETIME(ABaseCharacter, MovementDirectionY);
	DOREPLIFETIME(ABaseCharacter, Health);
	DOREPLIFETIME(ABaseCharacter, Shield);
	DOREPLIFETIME(ABaseCharacter, bDisableGameplay);	
}
#pragma endregion
#pragma endregion
#pragma region Camera
void ABaseCharacter::EliminationCamera(float DeltaTime)
{
	float RotateSpeed = 50.0f;
	FRotator NewRotation = DeathCamera->GetRelativeRotation();
	NewRotation.Yaw += RotateSpeed * DeltaTime;
	DeathCamera->SetRelativeRotation(NewRotation);

	float LiftoffSpeed = 200.0f;
	FVector NewLocation = DeathCamera->GetRelativeLocation();
	NewLocation.Z += LiftoffSpeed * DeltaTime;
	DeathCamera->SetRelativeLocation(NewLocation);
	DeathCamera->FieldOfView += 0.1f;
}
#pragma endregion
#pragma region Input
void ABaseCharacter::EnhancedInputLocalPlayerSubsystem()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BaseCharacterMappingContext, 0);
		}
	}
}

void ABaseCharacter::Input_SwitchCamera(const FInputActionValue& Value)
{
	if (GetPlayerIsEliminated() || GetInGameOptionMenuOpened()) return;
}

void ABaseCharacter::Input_Movement(const FInputActionValue& Value)
{
	if (GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened())
	{
		MovementDirectionX = 0.0f;
		MovementDirectionY = 0.0f;
		AddMovementInput(FVector::ZeroVector);
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		return;
	}
	else
	{
		if (Controller != NULL)
		{
			FVector2D MovementVector = Value.Get<FVector2D>();

			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			const FVector MovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);
			AddMovementInput(MovementDirection);

			MovementDirectionX = MovementVector.X;
			MovementDirectionY = MovementVector.Y;

			bool AvaliableForAimDirection =
				!HasAuthority() &&
				!EnhancedMovementComponent->IsFalling() &&
				GetVelocity().Size() > 0.0f &&
				IsAiming();

			if (AvaliableForAimDirection)
			{
				Server_SendMovemenntDirections(MovementDirectionX, MovementDirectionY);
			}
		}
	}
}

void ABaseCharacter::Input_Look(const FInputActionValue& Value)
{
	if (GetPlayerIsEliminated() || GetInGameOptionMenuOpened()) return;

	const FVector2D LookAxisVector	= Value.Get<FVector2D>();
	float ScaledPitchInput			= LookAxisVector.Y * -1.0f;
	float ScaledYawInput			= LookAxisVector.X;

	AddControllerPitchInput(ScaledPitchInput);
	AddControllerYawInput  (ScaledYawInput);
}

void ABaseCharacter::Input_Sprint(const FInputActionValue& Value)
{
	if (GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened()) return;

	if (EnhancedMovementComponent->CanSprint())
	{
		EnhancedMovementComponent->SwitchSprint();
		EnhancedMovementComponent->SetSprintInput(true);
	}
}

void ABaseCharacter::Input_Crouch_Activation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened()) return;

	if (EnhancedMovementComponent->CanCrouch())
	{
		EnhancedMovementComponent->SetCrouchInput(true);
	}
}

void ABaseCharacter::Input_Crouch_DeActivation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened()) return;

	EnhancedMovementComponent->SetCrouchInput(false);
}

void ABaseCharacter::Input_Jump(const FInputActionValue& Value)
{
	if (GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened() || EnhancedMovementComponent->IsCrouching()) return;

	if (IsLocallyControlled())
	{
		bPressedJump = true;
		if (CombatComponent->IsAiming())
		{
			CombatComponent->Set_Aiming(false);
		}
	}
	else
	{

	}
	Super::Jump();
}

void ABaseCharacter::Input_Fire_Activation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened()) return; 
	if (EnhancedMovementComponent->IsFalling()) return;

	bool CombatComponentReturnTypes = 
		CombatComponent == nullptr					||
		CombatComponent->EquippedWeapon == nullptr	|| 
		CombatComponent->IsAiming() == false		||
		CombatComponent->IsReloading()				||
		CombatComponent->IsSwapingWeapon();

	if (CombatComponentReturnTypes) return;

	if (CombatComponent->EquippedWeapon->Get_WeaponType() == EWeaponType::EWT_ScatterWeapons && CombatComponent->IsReloading())
	{
		if (bAttemptToCancelShotgunReloading != true)
			bAttemptToCancelShotgunReloading = true;
	}

	if (CombatComponent->EquippedWeapon->IsEmpty())
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			this,
			CombatComponent->EquippedWeapon->GetEmptyTriggerSound(),
			CombatComponent->EquippedWeapon->GetActorLocation()
		);
	}
	else
	{
		CombatComponent->Set_FireButton(true);
	}
}

void ABaseCharacter::Input_Fire_DeActivation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened()) return;

	if (CombatComponent->EquippedWeapon == nullptr || IsAiming() == false || EnhancedMovementComponent->IsFalling() || CombatComponent->IsReloading()) return;

	if (CombatComponent)
	{
		CombatComponent->Set_FireButton(false);
	}
}

void ABaseCharacter::Input_Interaction(const FInputActionValue& Value)
{
	if (GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened()) return;

	if (CombatComponent)
	{	
		Server_EquipButtonPressed();
	}
}

void ABaseCharacter::Input_EnterVehicleAction(const FInputActionValue& Value)
{
	if (AerodyneClass != nullptr)
	{
		EnterVehicle_Aerodyne(AerodyneClass);
	}
	else if (GroundVehiclePawn != nullptr)
	{
		EnterVehicle_GroundVehicle(GroundVehiclePawn);
	}
	else
	{
		return;
	}

	//Server_VehicleAction();	
}

void ABaseCharacter::Input_Reload(const FInputActionValue& Value)
{
	bool ReturnTypes = GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened();
	if (ReturnTypes) return;

	if (CombatComponent && !CombatComponent->CanReload()) return;

	CheckAndCancelShotgunReloadingIfAttempted();
	if (CombatComponent->IsReloading() == false)
	{
		CombatComponent->Reload();
	}
}

void ABaseCharacter::Input_SwapWeapon(const FInputActionValue& Value)
{
	bool ReturnTypes = GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened();
	if (ReturnTypes) return;

	if (CombatComponent && CombatComponent->CanSwapWeapons())
	{
		CombatComponent->DoSwap();
	}
}

void ABaseCharacter::Input_AimButton_Activation(const FInputActionValue& Value)
{
	bool ReturnTypes = GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened();
	if (ReturnTypes) return;

	CheckAndCancelShotgunReloadingIfAttempted();

	if (CombatComponent->IsReloading()) 
	{
		return;
	}
	UCharacterAnimInstance* CharacterAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (CharacterAnimInstance)
	{
		if (CharacterAnimInstance->AimOffSetIsHigh_DisableAim())
		{
			if (CombatComponent)
			{
				CombatComponent->Set_Aiming(false);
			}
		}
		else
		{
			if (CombatComponent)
			{
				CombatComponent->Set_Aiming(true);
			}
		}
	}
}

void ABaseCharacter::Input_AimButton_DeActivation(const FInputActionValue& Value)
{
	if (GameplayIsDisabled() || GetPlayerIsEliminated() || GetInGameOptionMenuOpened()) return;

	if (CombatComponent->IsReloading()) return;

	if (CombatComponent)
	{
		CombatComponent->Set_Aiming(false);
	}
}

void ABaseCharacter::InputAction_InGameOptionMenu()
{ 
	if (InGameOptionMenuWidgetClass == nullptr) return;

	bInGameOptionMenuOpened = !bInGameOptionMenuOpened;

	//FInputModeUIOnly InputModeData;
	if (bInGameOptionMenuOpened)
	{
		//PlayerControllerManager->SetInputMode(InputModeData);
		//InputModeData.SetWidgetToFocus(InGameOption->TakeWidget());
		PlayerControllerManager->SetShowMouseCursor(true);
		PlayerControllerManager->SetHUDInGameMenuOpened();
		InGameOption->SetVisibility(ESlateVisibility::Visible);
		bCacheInGameOptionMenuOpened = true;
	}
	else
	{
		PlayerControllerManager->SetShowMouseCursor(false);
		PlayerControllerManager->SetHUDInGameMenuClosed();
		InGameOption->SetVisibility(ESlateVisibility::Hidden);
		bCacheInGameOptionMenuOpened = false;
	}

	if (bCacheInGameOptionMenuOpened == false)
	{
		bInGameOptionMenuOpened = false;
	}
}

void ABaseCharacter::InputAction_Scoreboard()
{
	if (ScoreBoardWidgetClass == nullptr) return;

	bScoreboardOpened = !bScoreboardOpened;

	if (bScoreboardOpened)
	{
		ScoreboardWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

#pragma endregion
#pragma region Input-Networking

void ABaseCharacter::Server_SendMovemenntDirections_Implementation(float DirectionX, float DirectionY)
{
	if (GameplayIsDisabled() || GetPlayerIsEliminated()) return;

	MovementDirectionX = DirectionX;
	MovementDirectionY = DirectionY;
}
#pragma endregion
#pragma region Weapon

void ABaseCharacter::DropOrDestroy_Weapon(AWeapon* Weapon)
{
	if (Weapon == nullptr) return;

	if (Weapon->Get_DestroyWeapon())
	{
		Weapon->Destroy();
	}
	else
	{
		Weapon->Drop_Weapon();
	}
}

void ABaseCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	OverlappingWeapon = Weapon;

	if (OverlappingWeapon && PlayerControllerManager)
	{
		PlayerControllerManager->SetHUDWeaponType(OverlappingWeapon->Get_WeaponType(), true);
	}
}

bool ABaseCharacter::IsWeaponEquipped()
{
	return (CombatComponent && CombatComponent->EquippedWeapon);
}

AWeapon* ABaseCharacter::GetEquippedWeapon()
{
	if (CombatComponent == nullptr) return nullptr;

	return CombatComponent->EquippedWeapon;
}

bool ABaseCharacter::IsAiming()
{
	return (CombatComponent && CombatComponent->IsAiming());
}



void ABaseCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDWeaponType(OverlappingWeapon->Get_WeaponType(), true);
		}
	}

	if (LastWeapon)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDWeaponType(LastWeapon->Get_WeaponType(), false);
		}
	}
}

void ABaseCharacter::Server_EquipButtonPressed_Implementation()
{
	bool ReturnTypes = GameplayIsDisabled() || GetPlayerIsEliminated();
	if (ReturnTypes) return;

	if (CombatComponent)
	{
		if (OverlappingWeapon != nullptr)
		{
			CombatComponent->Equip_Weapon(OverlappingWeapon);
		}

		if (OverlappingFlag != nullptr)
		{
			if (CombatComponent->Get_HoldingFlag() == false) 
			{
				CombatComponent->Equip_Flag(OverlappingFlag);
			}
		}
	}
}

void ABaseCharacter::SpawnDefaultWeapon()
{
	if (bStartWithWeapon)
	{
		BaseGameMode = BaseGameMode == nullptr ? Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)) : BaseGameMode;

		UWorld* World = GetWorld();
		if (BaseGameMode && World && !GetPlayerIsEliminated() && DefaultWeaponClass)
		{
			AWeapon* StartingWeapon = World->SpawnActor<AWeapon>(DefaultWeaponClass);
			StartingWeapon->SetDestroyWeapon(true);

			if (CombatComponent)
			{
				CombatComponent->Equip_Weapon(StartingWeapon);
				CombatComponent->EquippedWeapon = StartingWeapon;
			}

			CameraManager = CameraManager == nullptr ? Cast<UCameraManager>(CameraManager) : CameraManager;
			if (CameraManager != nullptr)
			{
				CameraManager->InitialCrosshair();
			}

			//PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(Controller) : PlayerControllerManager;
			//if (PlayerControllerManager)
			//{
			//	PlayerControllerManager->SetHUDWeaponType(StartingWeapon->Get_WeaponType(), true);
			//}
			//else
			//{
			//	LOG_SCREEN("PCM is nullptr.! SpawnWeapon")
			//}
		}
	}
}

void ABaseCharacter::CheckAndCancelShotgunReloadingIfAttempted()
{

}

#pragma endregion
#pragma region Damage

FVector ABaseCharacter::GetHitTarget() const
{
	if (CombatComponent == nullptr) return FVector();
	return CombatComponent->HitTarget;
}

void ABaseCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	if (bPlayerEliminated) return;

	if (AttributeComponent->IsHealing())
	{
		AttributeComponent->SetHealing(false);
	}

	if (AttributeComponent->IsShieldRefreshing())
	{
		AttributeComponent->SetShieldRefreshing(false);
	}

	BaseGameMode = BaseGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABaseGameMode>() : BaseGameMode;

	if (GameModeClassName == "BP_CaptureTheFlagGameMode_C")
	{
		Damage = BaseGameMode->CalculateDamage(InstigatorController, Controller, Damage);
	}
	else if (GameModeClassName == "BP_DeathMatch_C")
	{
		Damage = GetEquippedWeapon()->GetDamage();
		LOG_WARNING("BP_DeathMatch_C")
	}
	else if (GameModeClassName == "BP_TeamMatchGameMode_C")
	{
		Damage = GetEquippedWeapon()->GetDamage();
		LOG_WARNING("BP_TeamMatchGameMode_C")

	}
	else if (GameModeClassName == "BP_Lobby_C")
	{
		Damage = GetEquippedWeapon()->GetDamage();
		LOG_WARNING("BP_Lobby_C")

	}

	float DamageToHealth = Damage;
	if (Shield > 0 && Health == MaxHealth)
	{
		if (Shield >= Damage)
		{
			Shield = FMath::Clamp(Shield - Damage, 0.0f, MaxShield);
			if (HasAuthority()) UpdateHUDShield(false);
			DamageToHealth = 0.0f;
		}
		else
		{
			DamageToHealth = FMath::Clamp(DamageToHealth - Shield, 0.0f, Damage);
			if (HasAuthority()) UpdateHUDShield(true);
			Shield = 0.0f;
		}
		bShieldBreakPlayable = true;
	}

	if (Shield <= 0.0f && bShieldBreakPlayable)
	{
		CombatComponent->ShieldBreak();
		bShieldBreakPlayable = false;
	}

	if (bShieldBreakPlayable == false && Health > 0.0f && Shield <= 0)
	{
		Health = FMath::Clamp(Health - DamageToHealth, 0.0f, MaxHealth);
		if (HasAuthority()) UpdateHUDHealth(false);
	}

	if (CombatComponent)
	{
		CombatComponent->PlayMontage_HitReaction();
	}

	if (Health <= 0.0f)
	{
		if (BaseGameMode)
		{
			PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(Controller) : PlayerControllerManager;

			APlayerControllerManager* AttackerController = Cast<APlayerControllerManager>(InstigatorController);
			BaseGameMode->PlayerEliminated(this, PlayerControllerManager, AttackerController);
			bPlayerEliminated = true;
			PlayerControllerManager->SetHUDEliminatedWindow();
		}
		if (HasAuthority()) UpdateHUDHealth(true);
	}
}
#pragma endregion
#pragma region Vehicles
void ABaseCharacter::EnterVehicle_Aerodyne(AAerodyne* VehicleClass)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		APawn* AerodynePawn = Cast<APawn>(VehicleClass);
		if (AerodynePawn)
		{
			AController* OwnerController = GetController();
			if (OwnerController)
			{
				OwnerController->Possess(AerodynePawn);
				VehicleClass->SetPlayerEnteredVehicle(true);
				VehicleClass->PossessedBy(OwnerController);

				APlayerController* AeroPlayerController = Cast<APlayerController>(OwnerController);
				UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(AeroPlayerController->GetLocalPlayer());
				if (Subsystem)
				{
					Subsystem->ClearAllMappings();
					Subsystem->AddMappingContext(VehicleClass->Get_AerodyneMappingContext(), 0);
				}
			}
			SetActorLocation(FVector(0, 0, 0));
		}
	}
	else if (GetLocalRole() < ROLE_Authority)
	{
		Server_EnterVehicle_Aerodyne(VehicleClass);
	}
}

void ABaseCharacter::Server_EnterVehicle_Aerodyne_Implementation(AAerodyne* VehicleClass)
{
	APawn* AerodynePawn = Cast<APawn>(VehicleClass);
	if (AerodynePawn)
	{
		AController* OwnerController = GetController();
		if (OwnerController)
		{
			OwnerController->Possess(AerodynePawn);
			VehicleClass->SetPlayerEnteredVehicle(true);
			VehicleClass->PossessedBy(OwnerController);

			APlayerController* AeroPlayerController = Cast<APlayerController>(OwnerController);
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(AeroPlayerController->GetLocalPlayer());
			if (Subsystem)
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(VehicleClass->Get_AerodyneMappingContext(), 0);
			}
		}
		SetActorLocation(FVector(0, 0, 0));
	}


	//VehicleClass->SetPlayerEnteredVehicle(true);
	//VehicleClass->SetOwner(this);
	//bPlayerInVehicle = true;

	//AController* VehicleRequesterController = GetController();
	//if (APlayerController* PlayerController = Cast<APlayerController>(VehicleRequesterController))
	//{
	//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//	{
	//		if (Subsystem)
	//		{
	//			Subsystem->AddMappingContext(VehicleClass->Get_AerodyneMappingContext(), 0);
	//		}
	//	}
	//}
	//if (VehicleRequesterController)
	//{
	//	VehicleRequesterController->Possess(VehicleClass);
	//	VehicleClass->SetOwner(VehicleRequesterController);
	//}
	//SetActorLocation(FVector(0, 0, 0));
}


//void ABaseCharacter::Multicast_VehicleAction_Aerodyne_Implementation(AAerodyne* VehicleClass, APlayerController* PC)
//{
//	VehicleClass->SetPlayerEnteredVehicle(true);
//	VehicleClass->CheckAeroEngineTypes();
//
//	if (HasAuthority())
//	{
//		PC->Possess(VehicleClass);
//	}
//
//	VehicleClass->SetOwner(this);
//
//	if (PC && PC->IsLocalPlayerController())
//	{
//		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
//		{
//			Subsystem->AddMappingContext(VehicleClass->Get_AerodyneMappingContext(), 0);
//		}
//	}
//}

void ABaseCharacter::EnterVehicle_GroundVehicle(AGroundVehiclePawn* GroundVehicle)
{
	if (GroundVehicle == nullptr) return;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		SetActorLocation(FVector(0, 0, 0));
		GroundVehicle->SetPlayerEnteredVehicle(true);
		bPlayerInVehicle = true;

		if (PlayerController)
		{
			PlayerController->Possess(GroundVehicle);
		}

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(GroundVehicle->Get_GroundVehicleInputMappingContext(), 0);
		}
	}
}


//void ABaseCharacter::Server_VehicleAction_Implementation()
//{
//	if (AerodyneClass == nullptr && GroundVehiclePawn == nullptr)
//	{
//		return;
//	}
//	else if (AerodyneClass != nullptr)
//	{
//		EnterVehicle_Aerodyne(AerodyneClass);
//	}
//	else if (GroundVehiclePawn != nullptr)
//	{
//		EnterVehicle_GroundVehicle(GroundVehiclePawn);
//	}
//}
#pragma endregion 
#pragma region CollisionProps
FCollisionQueryParams ABaseCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

void ABaseCharacter::SetCharacterMeshCollisionEnabled(ECollisionEnabled::Type NewType)
{
	GetHairMesh	()->SetCollisionEnabled(NewType);
	GetHeadMesh	()->SetCollisionEnabled(NewType);
	GetMesh		()->SetCollisionEnabled(NewType);
	GetLegsMesh	()->SetCollisionEnabled(NewType);
	GetArmsMesh	()->SetCollisionEnabled(NewType);
	GetHandsMesh()->SetCollisionEnabled(NewType);
}
#pragma endregion
#pragma region Elimination & CharacterIgnores
void ABaseCharacter::Elimination(bool bPlayerLeftGame)
{
	Multicast_Elimination(bPlayerLeftGame);
}

void ABaseCharacter::Multicast_Elimination_Implementation(bool bPlayerLeftGame)
{
	bLeftGame = bPlayerLeftGame;
	bool IsCameraComponentsValid =
		DeathCamera &&
		ThirdPersonCamera &&
		FirstPersonCamera;

	if (IsCameraComponentsValid)
	{
		FirstPersonCamera->	SetActive(false);
		ThirdPersonCamera->	SetActive(false);
		DeathCamera->		SetActive(true);
	}

	if (CombatComponent != nullptr)
	{
		if (CombatComponent->EquippedWeapon != nullptr)
		{
			DropOrDestroy_Weapon(CombatComponent->EquippedWeapon);
		}

		if (CombatComponent->SecondaryWeapon != nullptr)
		{
			DropOrDestroy_Weapon(CombatComponent->SecondaryWeapon);
		}

		if (CombatComponent->Get_HoldingFlag())
		{
			if (CombatComponent->EquippedFlag != nullptr)
			{
				//CombatComponent->Detach_FlagFromCharacters(CombatComponent->EquippedFlag);
				DropOrDestroy_Flag(CombatComponent->EquippedFlag);
			}
		}
	}

	SetRagdollValues(true, ECollisionEnabled::PhysicsOnly);

	if (DissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);

		GetMesh()->SetMaterial			(0, DynamicDissolveMaterialInstance);
		GetLegsMesh()->SetMaterial		(0, DynamicDissolveMaterialInstance);
		GetArmsMesh()->SetMaterial		(0, DynamicDissolveMaterialInstance);
		GetHandsMesh()->SetMaterial		(0, DynamicDissolveMaterialInstance);

		if (DissolveHeadMaterialInstance)
		{
			GetHeadMesh()->SetMaterial	(0, DynamicDissolveMaterialInstance);
		}

		if (DissolveHairMaterialInstance)
		{
			GetHairMesh()->SetMaterial	(0, DynamicDissolveMaterialInstance);
		}
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), 0.55f);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Glow"), 200.0f);
	}
	StartDissolve();

	// Disable Gameplay
	SetDisableGameplay(true);

	// Set Player Eliminated
	SetPlayerIsEliminated(true);

	// Disable Sniper scope widget if player is aiming with sniper rifle and killed
	bool HideSniperScopeIfIsValid = 
		   IsLocallyControlled()
		&& CombatComponent
		&& CombatComponent->IsAiming()
		&& CombatComponent->EquippedWeapon 
		&& CombatComponent->EquippedWeapon->Get_WeaponType() == EWeaponType::EWT_SniperRifle;

	if (HideSniperScopeIfIsValid)
	{
		ShowSniperScopeWidget(false);
	}

	GetWorldTimerManager().SetTimer
	(
		EliminationTimer,
		this,
		&ABaseCharacter::EliminationTimerFinished,
		EliminationTimerDelay
	);
}

void ABaseCharacter::DeathCondition(float DeltaTime)
{
	if (bPlayerEliminated)
	{
		if (bDeathProgressOver == false)
		{
			if (ThirdPersonCamera->IsActive())
			{
				ThirdPersonCamera->Deactivate();
			}
			else if (FirstPersonCamera->IsActive())
			{
				FirstPersonCamera->Deactivate();
			}

			if (DeathCamera->IsActive() == false)
			{
				DeathCamera->SetActive(true);
			}

			if (CombatComponent)
			{
				if (CombatComponent->IsFireButtonActive())
				{
					CombatComponent->Set_FireButton(false);
				}

				if (CombatComponent->IsAiming())
				{
					CombatComponent->Set_Aiming(false);
				}
			}
			if (InGameOption != nullptr)
			{
				InGameOption->RemoveFromParent();
			}
			bDeathProgressOver = true;
		}
		EliminationCamera(DeltaTime);
	}
}

void ABaseCharacter::SetRagdollValues(bool SimulatePhysics, ECollisionEnabled::Type NewType)
{
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	GetHairMesh()		->SetSimulatePhysics(SimulatePhysics);
	GetHeadMesh()		->SetSimulatePhysics(SimulatePhysics);
	GetMesh()			->SetSimulatePhysics(SimulatePhysics);
	GetLegsMesh()		->SetSimulatePhysics(SimulatePhysics);
	GetArmsMesh()		->SetSimulatePhysics(SimulatePhysics);
	GetHandsMesh()		->SetSimulatePhysics(SimulatePhysics);
	GetHairMesh()		->SetCollisionEnabled(NewType);
	GetHeadMesh()		->SetCollisionEnabled(NewType);
	GetMesh()			->SetCollisionEnabled(NewType);
	GetLegsMesh()		->SetCollisionEnabled(NewType);
	GetArmsMesh()		->SetCollisionEnabled(NewType);
	GetHandsMesh()		->SetCollisionEnabled(NewType);
}

void ABaseCharacter::EliminationTimerFinished()
{
	BaseGameMode = BaseGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABaseGameMode>() : BaseGameMode;

	if (BaseGameMode && !bLeftGame)
	{
		BaseGameMode->RequestRespawn(this, Controller);
	}
	if (bLeftGame && IsLocallyControlled())
	{
		OnLeftGame.Broadcast();
	}

	/*	bool SniperRifleIsValid =
		//IsLocallyControlled() &&
		//IsAiming() &&
		GetEquippedWeapon() &&
		GetEquippedWeapon()->Get_WeaponType() == EWeaponType::EWT_SniperRifle;*/  
	//if (SniperRifleIsValid)
	//{
	//	ShowSniperScopeWidget(false);
	//}
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

void ABaseCharacter::UpdateDissolveMaterial(float DissolveValue)
{
	if (DynamicDissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), DissolveValue);
	}
}
#pragma endregion
#pragma region Teams
void ABaseCharacter::SetTeamColor(ETeam Team)
{
	if (GetMesh() == nullptr || DefaultBodyMaterial == nullptr) return;

	if (IsLocallyControlled())
	{
		switch (Team)
		{
		case ETeam::ET_NoTeam:
			GetMesh()->SetMaterial(0, DefaultBodyMaterial);
			GetMesh()->SetMaterial(1, DefaultHeadMaterial);
			GetHairMesh()->SetMaterial(0, DefaultHairMaterial);
			GetHeadMesh()->SetMaterial(0, DefaultHeadMaterial);
			GetLegsMesh()->SetMaterial(0, DefaultBodyMaterial);
			GetArmsMesh()->SetMaterial(0, DefaultBodyMaterial);
			GetHandsMesh()->SetMaterial(0, DefaultBodyMaterial);
			DissolveMaterialInstance = BlackDissolveBodyMaterialInstance;
			break;
		case ETeam::ET_VanguardSovereignty:
			GetMesh()->SetMaterial(0, BlackBodyMaterial);
			GetMesh()->SetMaterial(1, BlackHeadMaterial);
			GetHairMesh()->SetMaterial(0, BlackHairMaterial);
			GetHeadMesh()->SetMaterial(0, BlackHeadMaterial);
			GetLegsMesh()->SetMaterial(0, BlackBodyMaterial);
			GetArmsMesh()->SetMaterial(0, BlackBodyMaterial);
			GetHandsMesh()->SetMaterial(0, BlackBodyMaterial);
			DissolveMaterialInstance = BlackDissolveBodyMaterialInstance;
			break;
		case ETeam::ET_GenesisPact:
			GetMesh()->SetMaterial(0, WhiteBodyMaterial);
			GetMesh()->SetMaterial(0, WhiteHeadMaterial);
			GetHairMesh()->SetMaterial(0, WhiteHairMaterial);
			GetHeadMesh()->SetMaterial(0, WhiteHeadMaterial);
			GetLegsMesh()->SetMaterial(0, WhiteBodyMaterial);
			GetArmsMesh()->SetMaterial(0, WhiteBodyMaterial);
			GetHandsMesh()->SetMaterial(0, WhiteBodyMaterial);
			DissolveMaterialInstance = WhiteDissolveBodyMaterialInstance;
			break;
		}
	}
}

ETeam ABaseCharacter::GetTeam()
{
	PlayerStateManager = PlayerStateManager == nullptr ? GetPlayerState<APlayerStateManager>() : PlayerStateManager;

	if (PlayerStateManager == nullptr) return ETeam::ET_NoTeam;

	return PlayerStateManager->GetTeam();
}



void ABaseCharacter::CheckOnPlayerStateInitialized()
{
	if (PlayerStateManager == nullptr)
	{
		PlayerStateManager = GetPlayerState<APlayerStateManager>();
		if (PlayerStateManager)
		{
			PlayerStateManager->AddToScore(0);
			PlayerStateManager->AddToAssists(0);
			PlayerStateManager->AddToKills(0);
			PlayerStateManager->AddToDeaths(0);
			SetTeamColor(PlayerStateManager->GetTeam());
			SetSpawnPoint();
		}
	}
}

void ABaseCharacter::SetSpawnPoint()
{
	if (HasAuthority() && PlayerStateManager->GetTeam() != ETeam::ET_NoTeam)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, ATeamPlayerStart::StaticClass(), PlayerStarts);
		TArray<ATeamPlayerStart*> TeamPlayerStarts;

		for (auto Start : PlayerStarts)
		{
			ATeamPlayerStart* TeamStart = Cast<ATeamPlayerStart>(Start);
			if (TeamStart && TeamStart->GetTeamFromTeamPlayerStart() == PlayerStateManager->GetTeam())
			{
				TeamPlayerStarts.Add(TeamStart);
			}
		}
		if (TeamPlayerStarts.Num() > 0)
		{
			ATeamPlayerStart* ChosenPlayerStart = TeamPlayerStarts[FMath::RandRange(0, TeamPlayerStarts.Num() - 1)];
			SetActorLocationAndRotation
			(
				ChosenPlayerStart->GetActorLocation(),
				ChosenPlayerStart->GetActorRotation()
			);
		}
	}
}
#pragma endregion
#pragma region CaptureTheFlag
void ABaseCharacter::DropOrDestroy_Flag(AFlag* DestroyOrDropFlag)
{
	if (DestroyOrDropFlag == nullptr) return;

	DestroyOrDropFlag->Drop_Flag();
}
void ABaseCharacter::Set_OverlappingFlag(AFlag* Flag)
{
	OverlappingFlag = Flag;
}
void ABaseCharacter::Set_HoldingFlag(bool bHolding)
{
	if (CombatComponent == nullptr) return;
	
	CombatComponent->bHoldingTheFlag = bHolding;
}
bool ABaseCharacter::IsHoldingTheFlag() const
{
	if (CombatComponent == nullptr) return false;
	return CombatComponent->bHoldingTheFlag;
}

#pragma endregion
#pragma region Character-Attributes

void ABaseCharacter::UpdateHUDHealth(bool bDead)
{
	CloseGameOptionMenuOnHit();
	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(GetController()) : PlayerControllerManager;

	if (PlayerControllerManager)
	{
		if (bDead == false)
		{
			PlayerControllerManager->SetHUDHealth(Health, MaxHealth);
		}
		else
		{
			PlayerControllerManager->SetHUDEliminatedWindow();
		}
	}
}

void ABaseCharacter::UpdateHUDShield(bool bShieldBreak)
{
	CloseGameOptionMenuOnHit();
	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(GetController()) : PlayerControllerManager;

	if (PlayerControllerManager)
	{	
		if (bShieldBreak == false)
		{
			PlayerControllerManager->SetHUDShield(Shield, MaxShield);
		}
		else
		{
			PlayerControllerManager->SetHUDShieldOff();
		}
	}

	//if (bInGameOptionMenuToggled)
	//{
	//	if (GetWorld())
	//	{
	//		InGameOptionMenuController = InGameOptionMenuController == nullptr ? Cast<APlayerControllerManager>(Controller) : InGameOptionMenuController;
	//		if (InGameOptionMenuController)
	//		{
	//			FInputModeGameOnly InputModeData;
	//			InGameOptionMenuController->SetInputMode(InputModeData);
	//			InGameOptionMenuController->SetShowMouseCursor(false);
	//			InGameOption->SetVisibility(ESlateVisibility::Hidden);
	//			InGameOptionMenuController->SetHUDInGameMenuClosed();
	//			bInGameOptionMenuToggled = false;
	//		}
	//	}
	//}
}

void ABaseCharacter::UpdateHUDAmmunition()
{
	PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(GetController()) : PlayerControllerManager;

	if (PlayerControllerManager)
	{
		if (CombatComponent->EquippedWeapon != nullptr)
		{
			PlayerControllerManager->SetHUDCarriedAmmo(CombatComponent->CarriedAmmo);
			PlayerControllerManager->SetHUDWeaponAmmo(CombatComponent->EquippedWeapon->Get_Ammo());
		}
		else
		{
			LOG_WARNING("COMBATCOMPONENT EQUIPPEDWEAPON IS NULLPTR");
		}
	}
}

void ABaseCharacter::OnRep_Health(float LastHealth)
{
	if (Health > 0)
	{
		UpdateHUDHealth(false);
	}
	else
	{
		UpdateHUDHealth(true);
	}

	if (Health < LastHealth)
	{
		if (CombatComponent)
		{
			CombatComponent->PlayMontage_HitReaction();
		}
	}	
}

void ABaseCharacter::OnRep_Shield(float LastShield)
{
	if (Shield > 0)
	{
		UpdateHUDShield(false);
	}
	else
	{
		UpdateHUDShield(true);
	}
	if (Shield < LastShield)
	{
		if (CombatComponent)
		{
			CombatComponent->PlayMontage_HitReaction();
		}
	}
}

#pragma endregion
#pragma region Gameplay
void ABaseCharacter::Server_LeaveGame_Implementation()
{
	BaseGameMode		= BaseGameMode		 == nullptr ? GetWorld()->GetAuthGameMode<ABaseGameMode>()  : BaseGameMode;
	PlayerStateManager	= PlayerStateManager == nullptr ? GetPlayerState<APlayerStateManager>()			: PlayerStateManager;

	if (BaseGameMode && PlayerStateManager)
	{
		BaseGameMode->PlayerLeftGame(PlayerStateManager);
	}
}
#pragma endregion
#pragma region Widgets
/*InGameOptionMenu*/
void ABaseCharacter::CreateInGameOptionMenu()
{
	APlayerController* InGameOptionMenuController = Cast<APlayerController>(Controller);

	if (InGameOptionMenuController == nullptr || InGameOptionMenuWidgetClass == nullptr)
	{
		return;
	}

	if (InGameOption == nullptr)
	{
		InGameOption = CreateWidget<UGameOption>(InGameOptionMenuController, InGameOptionMenuWidgetClass);
		if (InGameOption)
		{
			InGameOption->AddToViewport();
			InGameOption->MenuSetup();
			InGameOption->SetVisibility(ESlateVisibility::Hidden);
		}		
	}
}

void ABaseCharacter::CheckInGameOptionMenu()
{
	if (InGameOption)
	{
		if (InGameOption->IsVisible() == false)
		{
			if (GetInGameOptionMenuOpened() == false)
			{
				return;
			}
			SetInGameOptionMenuOpened(false);
		}
	}
}
void ABaseCharacter::CloseGameOptionMenuOnHit()
{
	if (bCacheInGameOptionMenuOpened)
	{
		PlayerControllerManager->SetShowMouseCursor(false);
		PlayerControllerManager->SetHUDInGameMenuClosed();
		InGameOption->SetVisibility(ESlateVisibility::Hidden);
		bCacheInGameOptionMenuOpened = false;
	}
}

/*Scoreboard*/
void ABaseCharacter::CreateScoreboardWidget()
{
	APlayerController* ScoreboardController = Cast<APlayerController>(Controller);

	if (ScoreboardController == nullptr || ScoreBoardWidgetClass == nullptr)
	{
		return;
	}

	if (ScoreboardWidget == nullptr)
	{
		ScoreboardWidget = CreateWidget<UScoreboard>(ScoreboardController, ScoreBoardWidgetClass);

		if (ScoreboardWidget)
		{
			ScoreboardWidget->AddToViewport();
			ScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}


}
#pragma endregion


#pragma region GameMode-Properties
FString ABaseCharacter::GetGameModeClassName()
{
	UWorld* World = GetWorld();
	if (World)
	{
		AWorldSettings* WorldSettings = World->GetWorldSettings();
		if (WorldSettings)
		{
			UClass* GameModeClass = WorldSettings->DefaultGameMode;
			if (GameModeClass)
			{
				FString SendGameModeName = GameModeClass->GetName();
				return SendGameModeName;
			}
		}
	}
	return FString();
}
#pragma endregion