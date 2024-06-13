// ©2023 Reverse-A. All rights reserved.

#include "BaseCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Synthetica/Components/CombatComponent.h"
#include "Synthetica/Characters/BaseAnimInstance.h"
#include "Synthetica/PlayerController/BasePlayerController.h"
#include "Synthetica/Weapons/Weapon.h"
#include "Math/UnrealMathUtility.h"
#include "Components/TimelineComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMeshCompenent = GetMesh();
	check(BaseMeshCompenent);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	bIsWeaponEquipped = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	BaseMeshCompenent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BaseMeshCompenent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BaseMeshCompenent->SetCollisionObjectType(ECC_GameTraceChannel1);

	BaseMeshCompenent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BaseMeshCompenent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	BaseMeshCompenent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BaseCapsuleCompenent = GetCapsuleComponent();
	BaseCapsuleCompenent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


	BaseInputController = CreateDefaultSubobject<APlayerController>(TEXT("PlayerController"));

	BaseMeshCompenent->SetGenerateOverlapEvents(true);

	CameraBoom = CreateDefaultSubobject< USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(BaseMeshCompenent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	FollowCamera->SetupAttachment(CameraBoom);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh(), FName("head"));
	FirstPersonCamera->bUsePawnControlRotation = true;


	AttachedHandGrenade = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Attached Grenade"));
	AttachedHandGrenade->SetupAttachment(GetMesh(), FName("RightHandGrenadeSocket"));
	AttachedHandGrenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));

	InitialWalkSpeed = 175.f;
	JogWalkSpeed = 400.f;
	SprintWalkSpeed = 900.f;
	CurrentWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	bIsCrouching = false;
	bSprintButtonPressed = false;
	bIsInAir = GetCharacterMovement()->IsFalling();
	bIsReload = false;

	RotateStatementEnum = ERotateStatement::ERS_None;

}


void ABaseCharacter::SwitchCameraMethod()
{
	if (FollowCamera->IsActive())
	{
		FirstPersonCamera->Activate();
		FollowCamera->Deactivate();
	}
	else
	{
		FirstPersonCamera->Deactivate();
		FollowCamera->Activate();
	}
}

void ABaseCharacter::SwitchCamera()
{
	GetWorldTimerManager().ClearTimer(CameraSwitchTimerHandle);
	GetWorldTimerManager().SetTimer(CameraSwitchTimerHandle, this, &ABaseCharacter::SwitchCameraMethod, .1f, false);
}


void ABaseCharacter::SetOverLappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}

	OverlappingWeapon = Weapon;

	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("BaseCharacter"));

	if (AttachedHandGrenade)
	{
		AttachedHandGrenade->SetVisibility(false);
	}

	BasePlayerController = Cast<ABasePlayerController>(Controller);
	if (BasePlayerController)
	{
		BasePlayerController->SetHUDHealth(Health, MaxHealth);
	}
	CurrentCharacterState = EWeaponEquippedStatus::ECS_Disarmed;


	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
	CurrentWalkSpeed = InitialWalkSpeed;

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
	CharacterGeneralCondition(DeltaTime);
}


void ABaseCharacter::CharacterGeneralCondition(float DeltaTime)
{
	AimOffset(DeltaTime);
	CharacterMovementState(DeltaTime);
	CalculatingPitchSlope(DeltaTime);
	WallAndObstacleDetactor(DeltaTime);


	if (CurrentMovementSpeed == 0)
	{
		CalculateFootSocketsFromGround(DeltaTime);
		if (bIsMoving)
			bIsMoving = false;
	}

	//if (bIsInAir && bIsJumping)
	//{
	//	if (bSprintJump || bNormalJumping)
	//	{
	//		return;
	//	}
	//	else
	//	{
	//		if (!bCanFall)
	//			bCanFall = true;
	//	}

	//}

	if (bCanFall)
	{
		DistanceFromGround();

		if (CharacterAndGroundBetweenDistance < 200.f)
		{
			if (!bCanRagDoll)
				bCanRagDoll = true;
		}
	}

	if (bCanRagDoll)
		RagDoll();
	if (bRagDolled)
		IfActorRagDoll(DeltaTime);

	if (bIsJumping && GetCharacterMovement()->IsMovingOnGround())
	{
		bCanMove = false;
		CurrentWalkSpeed = 0.f;
		bCanMoveAfterJump = false;
		bIsJumping = false;
	}

	if (!bCanMoveAfterJump)
	{
		bJumpIsDone = true;
		WaitForMovementAfterJumpDone_Delay += DeltaTime;
		if (bNormalJumping)bNormalJumping = false;
		if (bSprintJump) bSprintJump = false;
		if (WaitForMovementAfterJumpDone_Delay >= 0.4f)
		{
			bCanMove = true;
			bCanJump = true;
			WaitForMovementAfterJumpDone_Delay = 0.f;
			CurrentWalkSpeed = 0.f;
			bCanMoveAfterJump = true;
		}
	}
}

void ABaseCharacter::CharacterMovementState(float DeltaTime)
{
	CurrentMovementSpeed = GetVelocity().Size();

	if(bIsReload || bThrowningGrenade) 	Combat->SetAiming(false);

	if (CurrentCharacterState == EWeaponEquippedStatus::ECS_Disarmed)
	{
		if (CurrentCharacterState == EWeaponEquippedStatus::ECS_Armed) return;
			
		if (!bSprintButtonPressed)
		{
			if (bIsSprinting) bIsSprinting = false;
			if(CurrentWalkSpeed != InitialWalkSpeed) CurrentWalkSpeed = InitialWalkSpeed;
			if(bCanSprint_Disarmed)bCanSprint_Disarmed = false;
		}
		else
		{
			if(!bIsSprinting) bIsSprinting = true;
			if (CurrentWalkSpeed <= JogWalkSpeed && !bCanSprint_Disarmed)
			{
				CurrentWalkSpeed = InterpolateSpeed(CurrentWalkSpeed, JogWalkSpeed, DeltaTime, 1.3f);

				if (CurrentWalkSpeed >= (JogWalkSpeed - 75.f)) bCanSprint_Disarmed = true;
			}

			if (CurrentWalkSpeed <= SprintWalkSpeed && bCanSprint_Disarmed)
			{
				CurrentWalkSpeed = InterpolateSpeed(CurrentWalkSpeed, SprintWalkSpeed, DeltaTime, 1.f);
			}
			
		}
	}

	if (CurrentCharacterState == EWeaponEquippedStatus::ECS_Armed)
	{
		if (CurrentCharacterState == EWeaponEquippedStatus::ECS_Disarmed) return;

		if (IsAiming())
		{
			if (!bIsCrouching)
			{
				if (CurrentWalkSpeed != InitialWalkSpeed)
					CurrentWalkSpeed = InitialWalkSpeed;

					GetCharacterMovement()->bOrientRotationToMovement = false;
			}
			else
			{
				if (!bSprintButtonPressed || bSprintButtonPressed)
				{
					if (CurrentWalkSpeed != CrouchWalkSpeed)
						CurrentWalkSpeed = CrouchWalkSpeed;
					if (GetCharacterMovement()->bOrientRotationToMovement != true)
						GetCharacterMovement()->bOrientRotationToMovement = true;
				}
			}
		}
		else
		{
			if (bVeryHighSlope)
			{
				CurrentWalkSpeed = JogWalkSpeed - 100.f;
			}
			else
			{
				if (bIsCrouching)
				{
					if (!bSprintButtonPressed || bSprintButtonPressed)
					{
						if (CurrentWalkSpeed != CrouchWalkSpeed)
							CurrentWalkSpeed = CrouchWalkSpeed;
						if (GetCharacterMovement()->bOrientRotationToMovement != true)
							GetCharacterMovement()->bOrientRotationToMovement = true;
					}
				}
				else
				{
					if (CurrentWalkSpeed == 0 || CurrentWalkSpeed == InitialWalkSpeed)
					{

						if (CurrentWalkSpeed != JogWalkSpeed)
							CurrentWalkSpeed = JogWalkSpeed;
					}
					if (GetCharacterMovement()->bOrientRotationToMovement != true)
						GetCharacterMovement()->bOrientRotationToMovement = true;
					if (bSprintButtonPressed)
					{
						
						if (CurrentWalkSpeed >= JogWalkSpeed)
						{
							if (CurrentWalkSpeed > SprintWalkSpeed)	return;
							if (bIsSprinting != true) bIsSprinting = true;
							CurrentWalkSpeed = InterpolateSpeed(CurrentWalkSpeed, SprintWalkSpeed, DeltaTime, .5f);
						}
						else if(CurrentWalkSpeed < JogWalkSpeed)
						{
							CurrentWalkSpeed = JogWalkSpeed;
						}
					}
					else
					{		
						if (bSprintButtonPressed) return;
						if (CurrentMovementSpeed == 0)
						{
							CurrentWalkSpeed = 50.f;

							if(!bcanStartJog)bcanStartJog = true;

						}
						if (bcanStartJog && bIsMoving)
						{
							if (CurrentWalkSpeed < 150.f)
							{
								CurrentWalkSpeed = InterpolateSpeed(CurrentWalkSpeed, JogWalkSpeed, DeltaTime, 1.f);
								CameraBoom->bEnableCameraLag = true;
								CameraBoom->CameraLagSpeed = 20.f;

								if (CurrentWalkSpeed >= 150.f)
								{
									CurrentWalkSpeed = 200.f;
								}
							}
							if (CurrentWalkSpeed >= 200.f)
							{
								CurrentWalkSpeed = InterpolateSpeed(CurrentWalkSpeed, JogWalkSpeed, DeltaTime, 1.3f);
								if (CurrentWalkSpeed >= 300.f)
								{
									bcanStartJog = true;
								}
								if (bcanStartJog)
								{
									CurrentWalkSpeed = InterpolateSpeed(CurrentWalkSpeed, JogWalkSpeed, DeltaTime, 1.3f);
								}
							}
							if (CurrentWalkSpeed >= 350.f)
							{
								CurrentWalkSpeed = JogWalkSpeed;
								CameraBoom->CameraLagSpeed  = InterpolateSpeed(CameraBoom->CameraLagSpeed, 0.f, DeltaTime, 1.f);
								bcanStartJog = false;
							}
						}
						if (bIsSprinting != false) bIsSprinting = false;

						const float SprintingInProgressStartLimit = 600.f;
						if (CurrentMovementSpeed >= SprintingInProgressStartLimit)
						{
							CurrentWalkSpeed = InterpolateSpeed(CurrentWalkSpeed, JogWalkSpeed, DeltaTime, .5f);
						}
					}
				}
			}
		}
	}
}

void ABaseCharacter::RagDoll()
{
	if (Combat)Combat->IfWeaponEquipped_DropEquippedWeapon();
	LastRagdollPosition = GetActorLocation();

	this->GetMesh()->SetSimulatePhysics(true);
	this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	bIsJumping = false;
	bJumpIsDone = true;
	bIsNotTouchTheGroundAfterJump = false;
	JumpingAfterTimerDelay = 0.f;
	bRagDolled = true;
	bCanRagDoll = false;
}

void ABaseCharacter::IfActorRagDoll(float DeltaTime)
{
	RagDollTimerDelay += DeltaTime;
	if (RagDollTimerDelay > 2.f)
	{
		RagDollTimerDelay = 0.f;
		this->Destroy();
		RespawnPlayer(LastRagdollPosition, GetActorRotation());
		bRagDolled = false;
	}
}


void ABaseCharacter::WallAndObstacleDetactor(float DeltaTime)
{
	FHitResult WallHitResult;
	FHitResult WallFootHitResult;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + GetActorForwardVector() * 70.f;

	FVector ActorFootStartLocation = FVector(StartLocation.X, StartLocation.Y, StartLocation.Z - 80.f);
	FVector ActorFootEndLocation = ActorFootStartLocation + GetActorForwardVector() * 35.f;

	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	GetWorld()->LineTraceSingleByChannel(WallHitResult, StartLocation, EndLocation, TraceChannel, TraceParams);
	GetWorld()->LineTraceSingleByChannel(WallFootHitResult, ActorFootStartLocation, ActorFootEndLocation, TraceChannel, TraceParams);

	bool bActorHitAnyStaticObject;
	WallHitResult.bBlockingHit || WallFootHitResult.bBlockingHit ? bActorHitAnyStaticObject = true : bActorHitAnyStaticObject = false;


	if (bActorHitAnyStaticObject && (CurrentMovementSpeed >= 750.f || bSprintJump))
	{
		if (bSlopeHitInRange) return;
		else
		{
			bCanRagDoll = true;
		}
	}
}

void ABaseCharacter::WallSlide()
{
	FVector ActorLocation    = GetActorLocation();
	FRotator ActorRotation   = GetActorRotation();
	FVector FrontEndLocation = ActorLocation + (ActorRotation.RotateVector(FVector::ForwardVector) * 100.f);

	FHitResult WallSlideHit;

	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	GetWorld()->LineTraceSingleByChannel(WallSlideHit, ActorLocation, FrontEndLocation, TraceChannel, TraceParams);

	if (WallSlideHit.bBlockingHit)
	{
		GetCharacterMovement()->GravityScale = 0.1f;
		bIsActorHitFrontWall = true;
	}
	else
	{
		GetCharacterMovement()->GravityScale = 1.f;
		bIsActorHitFrontWall = false;
	}
}

void ABaseCharacter::RespawnPlayer(FVector SpawnLocation, FRotator SpawnRotation)
{
	FActorSpawnParameters SpawnParams;
	ABaseCharacter* SpawnedCharacter = GetWorld()->SpawnActor<ABaseCharacter>(GetClass(), SpawnLocation, SpawnRotation, SpawnParams);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->Possess(SpawnedCharacter);
	}
}

float ABaseCharacter::InterpolateSpeed(float CurrentSpeed, float TargetSpeed, float DeltaTime, float InterpolationSpeed)
{
	float NewSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, InterpolationSpeed);
	return NewSpeed;
}


void ABaseCharacter::CalculateFootSocketsFromGround(float DeltaTime)
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();

		if (SkeletalMeshComponent)
		{
			FName LeftSocketName = FName(TEXT("CalculatingGround_L"));
			FName RightSocketName = FName(TEXT("CalculatingGround_R"));

			FTransform LeftSocketTransform = SkeletalMeshComponent->GetSocketTransform(LeftSocketName, ERelativeTransformSpace::RTS_World);
			FTransform RightSocketTransform = SkeletalMeshComponent->GetSocketTransform(RightSocketName, ERelativeTransformSpace::RTS_World);

			FVector LeftSocketLocation = LeftSocketTransform.GetLocation();
			FVector RightSocketLocation = RightSocketTransform.GetLocation();

			float Difference = FMath::Abs(LeftSocketLocation.Z - RightSocketLocation.Z);
			DifferenceBetweenTwoFeet = Difference;

			float lineLenght = 150.f;
			FVector LineDirection = FVector(0.f, 0.f, -1.f);

			FVector LeftSocketEndLocation = LeftSocketLocation + LineDirection * lineLenght;
			FVector RightSocketEndLocation = RightSocketLocation + LineDirection * lineLenght;

			FHitResult LeftHitResult;
			FHitResult RightHitResult;
			ECollisionChannel CollisionChannel = ECollisionChannel::ECC_Visibility;
			FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, this);

			// Calculate the left foot how far from ground

			GetWorld()->LineTraceSingleByChannel(LeftHitResult, LeftSocketLocation, LeftSocketEndLocation, CollisionChannel, TraceParams);
			// Check if the line trace hit something
			if (LeftHitResult.bBlockingHit)
			{
				// Calculate the distance from the start location to the hit location
				float DistanceToGround = FVector::Distance(LeftSocketLocation, LeftHitResult.Location);
				LeftDistanceToGround = DistanceToGround;

			}
			GetWorld()->LineTraceSingleByChannel(RightHitResult, RightSocketLocation, RightSocketEndLocation, CollisionChannel, TraceParams);
			if (RightHitResult.bBlockingHit)
			{
				// Calculate the distance from the start location to the hit location
				float DistanceToGround = FVector::Distance(RightSocketLocation, RightHitResult.Location);
				RightDistanceToGround = DistanceToGround;
			}

		}
	}
}

void ABaseCharacter::DistanceFromGround()
{
	if (bIsHoldingWall) return;
	FHitResult CharacterBetweenGroundHitResult;

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (GetActorForwardVector() + FVector(0.f, 0.f, -5000.f));

	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_Visibility;
	FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, this);


	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Cyan, false, -1, 0, 2.f);

	GetWorld()->LineTraceSingleByChannel(CharacterBetweenGroundHitResult, StartLocation, EndLocation, CollisionChannel, TraceParams);
	if (CharacterBetweenGroundHitResult.bBlockingHit)
	{
		// Calculates the distance between the "StartLocation" and the location of the character and ground intersection.
		CharacterAndGroundBetweenDistance = FVector::Distance(StartLocation, CharacterBetweenGroundHitResult.Location);

		if (bIsJumping)
		{
			if (CharacterAndGroundBetweenDistance > 1000.f)
			{
				if (!bCanFall)
					bCanFall = true;

				bIsJumping = false;
			}
			else
			{
				if (bCanFall)
					bCanFall = false;
			}
		}
	}
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	if (bIsJumping || !bCanMove) return;
	bIsMoving = true;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//AddMovementInput(RightDirection, MovementVector.X);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector MovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);
	AddMovementInput(MovementDirection);

	MovementX = MovementVector.X;
	MovementY = MovementVector.Y;
}

void ABaseCharacter::MoveNegative(const FInputActionValue& Value)
{
	if(bIsMoving)
		bIsMoving = false;
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ABaseCharacter::JumpFunction(const FInputActionValue& Value)
{
	if (bIsCrouching) return;

	if (bIsHoldingWall)
	{
		GetCharacterMovement()->GravityScale = 1.f;
		FVector ActorForward		= GetActorForwardVector() * -350.f;
		FVector ActorOppositeVector = ActorForward + FVector(0.f, 0.f, 500.F);
		LaunchCharacter(ActorOppositeVector, false, true);
		FRotator NewRotation		= FRotator(0.f, GetActorRotation().Yaw + 180.f, 0.f);
		SetActorRotation(NewRotation);
		bIsJumping = true;
		bIsHoldingWall = false;
	}


	if (GetCharacterMovement()->IsMovingOnGround() && CurrentMovementSpeed < 600.f && bCanJump && !bIsJumping)
	{
		if (bSprintJump || bIsHoldingWall) return;

		bNormalJumping = true;

		float JumpValue = GetCharacterMovement()->JumpZVelocity;
		FVector JumpVector = FVector(0.f, 0.f, JumpValue);
		LaunchCharacter(JumpVector, false, true);
		bIsJumping = true;
		bCanJump = false;
	}

	if (GetCharacterMovement()->IsMovingOnGround() && CurrentMovementSpeed > 600.f && bCanJump && !bIsJumping)
	{
		if (bNormalJumping || bIsHoldingWall) return;
		bSprintJump = true;

		float JumpValue = GetCharacterMovement()->JumpZVelocity * 0.6f;
		FVector JumpVector = FVector(20.f, 0.f, JumpValue);
		LaunchCharacter(JumpVector, false, true);
	}
	bJumpIsDone = false;
}

void ABaseCharacter::SecondJumpTrigger()
{
	if (bSprintJump)
	{
		DistanceFromGround();
		bool bCheckFeetAreTouchingGround;
		if (CharacterAndGroundBetweenDistance > 500.f)
		{
			bCheckFeetAreTouchingGround = false;
			bCanFall = true;
		}
		else
		{
			bCheckFeetAreTouchingGround = true;
		}

		if (bCheckFeetAreTouchingGround)
		{
			bIsJumping = true;
			float JumpValue = GetCharacterMovement()->JumpZVelocity * 1.5f;
			FVector JumpVector = FVector(0.f, 0.f, JumpValue);
			LaunchCharacter(JumpVector, false, true);
			bCanJump = false;
		}
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(JumpAction,		ETriggerEvent::Triggered, this, &ABaseCharacter::JumpFunction);
		
		EnhancedInputComponent->BindAction(CrouchAction,	ETriggerEvent::Triggered, this, &ABaseCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction,	ETriggerEvent::Completed, this, &ABaseCharacter::StopCrouch);

		EnhancedInputComponent->BindAction(MoveAction,		ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction,		ETriggerEvent::Completed, this, &ABaseCharacter::MoveNegative);
		EnhancedInputComponent->BindAction(LookAction,		ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
		EnhancedInputComponent->BindAction(SprintAction,	ETriggerEvent::Triggered, this, &ABaseCharacter::StartSprinting);
		EnhancedInputComponent->BindAction(SprintAction,	ETriggerEvent::Completed, this, &ABaseCharacter::StopSprinting);

		EnhancedInputComponent->BindAction(AimAction,		ETriggerEvent::Triggered, this, &ABaseCharacter::AimButtonPressed);
		EnhancedInputComponent->BindAction(AimAction,		ETriggerEvent::Completed, this, &ABaseCharacter::AimButtonReleased);

		EnhancedInputComponent->BindAction(FireAction,		ETriggerEvent::Triggered, this, &ABaseCharacter::FireButtonPressed);
		EnhancedInputComponent->BindAction(FireAction,		ETriggerEvent::Completed, this, &ABaseCharacter::FireButtonReleased);

		EnhancedInputComponent->BindAction(InteractionAction,	ETriggerEvent::Triggered, this, &ABaseCharacter::InteractionButtonPressed);
		EnhancedInputComponent->BindAction(InteractionAction,	ETriggerEvent::Completed, this, &ABaseCharacter::InteractionButtonReleased);

		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &ABaseCharacter::ToggleWeapon);

		EnhancedInputComponent->BindAction(SwitchCameraAction,	ETriggerEvent::Triggered, this, &ABaseCharacter::SwitchCamera);
		EnhancedInputComponent->BindAction(ReloadAction,		ETriggerEvent::Triggered, this, &ABaseCharacter::ReloadButton);

		EnhancedInputComponent->BindAction(ThrowGrenadeAction, ETriggerEvent::Triggered, this, &ABaseCharacter::ThrowGrenadeButton);
	}
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	{
		if (Combat)
			Combat->BaseCharacter = this;
	}
}


void ABaseCharacter::PlayFireMontage(bool bAiming)
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}



void ABaseCharacter::AimButtonPressed(const FInputActionValue& Value)
{
	//FollowCamera->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	if (bIsReload || bThrowningGrenade || bIsInAir)  return;

	if (Combat && Combat->EquippedWeapon)
	{
		if(!bIsSprinting)
		Combat->SetAiming(true);
	}
}
void ABaseCharacter::AimButtonReleased(const FInputActionValue& Value)
{
	//FollowCamera->SetRelativeLocation(FVector(350.f, 0.f, 0.f));

	if (Combat && Combat->EquippedWeapon)
	{
		Combat->SetAiming(false);
	}
}

void ABaseCharacter::InteractionButtonPressed(const FInputActionValue& Value)
{
	FVector Velocity = GetVelocity();
	if (Velocity.Z < -100.f)
	{
		WallSlide();
	}

	if (bIsActorHitFrontWall) // Updates the "bIsHoldingWall" variable based on whether the actor has hit the front wall or not.
	{
		if(!bIsHoldingWall)
		bIsHoldingWall = true;

		if (bIsJumping)	
			bIsJumping = false;
	}
	else
	{
		if (bIsHoldingWall)
			bIsHoldingWall = false;
	}


	if (Combat)
	{
		if (OverlappingWeapon == nullptr) return;
		Combat->EquipWeapon(OverlappingWeapon);
	}
	
}

void ABaseCharacter::InteractionButtonReleased(const FInputActionValue& Value)
{	
	if (bIsHoldingWall)
		bIsHoldingWall = false;

	if(GetCharacterMovement()->GravityScale != 1.f)
		GetCharacterMovement()->GravityScale = 1.f;

}
void ABaseCharacter::StartCrouch(const FInputActionValue& Value)
{
	if (!bIsCrouching)
	{
		bIsCrouching = true;
		//BaseCapsuleCompenent->SetCapsuleHalfHeight(88.f);
	}
}
void ABaseCharacter::StopCrouch(const FInputActionValue& Value)
{
	if (bIsCrouching)
	{
		bIsCrouching = false;
		//BaseCapsuleCompenent->SetCapsuleHalfHeight(88.f);
		if (CurrentCharacterState == EWeaponEquippedStatus::ECS_Disarmed)
		{
			CurrentWalkSpeed = InitialWalkSpeed;
		}
		else if(CurrentCharacterState == EWeaponEquippedStatus::ECS_Armed)
		{
			CurrentWalkSpeed = JogWalkSpeed;
		}
	}	
}

void ABaseCharacter::FireButtonPressed()
{
	if (bIsReload || bThrowningGrenade)  return;

	bIsFireButtonTriggered = true;
	if (Combat)
	{
		if(Combat->bCanFire)
		Combat->FireButtonPressed(true);
	}
}
void ABaseCharacter::FireButtonReleased()
{
	if (bIsReload || bThrowningGrenade) return;
	bIsFireButtonTriggered = false;
	if (Combat && !bIsReload)
	{
		Combat->FireButtonPressed(false);
	}
}

void ABaseCharacter::ReloadButton()
{
	if (Combat->EquippedWeapon == nullptr || bIsInAir || 
		Combat->EquippedWeapon->IsAmmoFull() ||
		Combat->CombatState == ECombatState::ECS_ThrowingGrenade || bIsReload) return;

	if (bIsWeaponEquipped && !bIsReload && Combat)
	{
		Combat->CombatState = ECombatState::ECS_Reloading;
		if(Combat->CarriedAmmo > 0)
		bIsReload = true;
	}
}

void ABaseCharacter::ReloadFinished()
{
	if (!bIsReload) return;
	Combat->UpdateCarriedAmmunition();
	Combat->CombatState = ECombatState::ECS_Null;
	bIsReload = false;
}


void ABaseCharacter::ThrowGrenadeButton()
{
	if (bIsInAir || bThrowningGrenade || GetEquippedWeapon() == nullptr || Combat->Grenades == 0) return;
	if (Combat)
	{
		if (!bThrowningGrenade)
		{
			if (bThrowningGrenade) return;

			bThrowningGrenade = true;
			Combat->ThrowGrenade();
		}
	}
}


void ABaseCharacter::StartSprinting()
{
	if (!bSprintButtonPressed)
	{
		bSprintButtonPressed = true;
	}
}
void ABaseCharacter::StopSprinting()
{
	if (bSprintButtonPressed)
	{
		bSprintButtonPressed = false;
	}
}

void ABaseCharacter::ToggleWeapon()
{
	if (GetWorldTimerManager().IsTimerActive(WeaponToggleTimerHandle)) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (Combat->EquippedWeapon)
	{
		if (!bIsWeaponEquipped)
		{
			CurrentCharacterState = EWeaponEquippedStatus::ECS_Armed;
			AnimInstance->Montage_Play(SwitchWeaponMontage);
			bIsWeaponEquipped = true;
			if (Combat->DisarmedWeapon)
			{
				Combat->EquipWeapon(DisarmedWeapon);
			}
		}
		else
		{
			if (Combat->EquippedWeapon)
			{
				Combat->UnEquipWeapon(DisarmedWeapon);
				DisarmedWeapon = Combat->EquippedWeapon;
				AnimInstance->Montage_Play(SwitchWeaponMontage);
				bIsWeaponEquipped = false;
			}
		}

		if (AnimInstance)
		{
			AnimInstance->Montage_Play(SwitchWeaponMontage);
			FName SectionName;
			switch (CurrentCharacterState)
			{
			case EWeaponEquippedStatus::ECS_Disarmed:
				SectionName = FName("DisarmWeapon");
				break;
			case EWeaponEquippedStatus::ECS_Armed:
				SectionName = FName("ArmWeapon");
				break;
			}
			AnimInstance->Montage_JumpToSection(SectionName);
		}
		GetWorldTimerManager().SetTimer(WeaponToggleTimerHandle, this, &ABaseCharacter::ResetWeaponToggleTimer, .5f, false);
	}
}

void ABaseCharacter::ResetWeaponToggleTimer()
{
	GetWorldTimerManager().ClearTimer(WeaponToggleTimerHandle);
}


void ABaseCharacter::CalculatingPitchSlope(float DeltaTime)
{

	FVector ActorLocation = GetActorLocation();
	FVector StartLocation = ActorLocation + (GetActorForwardVector() * 35.f);
	FVector End = StartLocation + (GetActorForwardVector() + FVector(0.f, 0.f, -500.f));

	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = false;
	ECollisionChannel TraceChannelVisibility = ECollisionChannel::ECC_Visibility;
	TraceParams.AddIgnoredActor(this);

	FHitResult HitResult;
	UWorld* World = GetWorld();
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, End, TraceChannelVisibility, TraceParams);

	bool bBlockingHit, bInitialOverlap;
	float Time, Distance;
	FVector Location, ImpactPoint, Normal, ImpactNormal, TraceStart, TraceEnd;
	class UPhysicalMaterial* PhysMat;
	class AActor* HitActor;
	class UPrimitiveComponent* HitComponent;
	FName HitBoneName, BoneName;
	int32 HitItem, ElementIndex, FaceIndex;

	UGameplayStatics::BreakHitResult(
		HitResult,
		bBlockingHit,
		bInitialOverlap,
		Time,
		Distance,
		Location,
		ImpactPoint,
		Normal,
		ImpactNormal,
		PhysMat,
		HitActor,
		HitComponent,
		HitBoneName,
		BoneName,
		HitItem,
		ElementIndex,
		FaceIndex,
		TraceStart,
		TraceEnd
	);


	FVector Right = GetActorRightVector(); 
	FVector Up = GetActorUpVector(); 
	UKismetMathLibrary::GetSlopeDegreeAngles(Right, ImpactNormal, Up, OutSlopePitchDegreeAngle, OutSlopeRollDegreeAngle);


	if (HitResult.bBlockingHit &&
		((OutSlopePitchDegreeAngle > 25.f && OutSlopePitchDegreeAngle < 39.f) ||
		(OutSlopePitchDegreeAngle < -25.f && OutSlopePitchDegreeAngle > -39.f)) && 
		!bSlopeHitInRange)
	{
		if(InSlopeMovement == false) InSlopeMovement = true;
		if(bVeryHighSlope == true) bVeryHighSlope = false;
		if(!bSlopeHitInRange) bSlopeHitInRange = true;
	}
	else if (HitResult.bBlockingHit && (OutSlopePitchDegreeAngle > 39.f || OutSlopePitchDegreeAngle < -39.f) && !bSlopeHitInRange)
	{
		if (InSlopeMovement == false) InSlopeMovement = true;
		if (bVeryHighSlope == false) bVeryHighSlope = true;
		if (!bSlopeHitInRange) bSlopeHitInRange = true;
	}
	else if (HitResult.bBlockingHit && OutSlopePitchDegreeAngle == 0 && bSlopeHitInRange)
	{
		SlopeDegreeResetTimerDelay += DeltaTime;

		if (SlopeDegreeResetTimerDelay >= .5f)
		{
			SlopeDegreeResetTimerDelay = 0.f;
			if(bVeryHighSlope) bVeryHighSlope = false;
			if(InSlopeMovement)InSlopeMovement = false;
		}
		if (!InSlopeMovement)
		{
			if(bSlopeHitInRange)bSlopeHitInRange = false;
			SlopeDegreeResetTimerDelay = 0.f;
		}
	}
}


float ABaseCharacter::GetSlopePitchDegreeAngle() const
{
	// Returns the calculated slope pitch degree angle from the last update of the character's pitch slope calculation.
	return OutSlopePitchDegreeAngle;
}
float ABaseCharacter::GetSlopRollDegreeAngle() const
{
	// Returns the calculated slope roll degree angle from the last update of the character's pitch slope calculation.
	return OutSlopeRollDegreeAngle;
}

void ABaseCharacter::AimOffset(float DeltaTime)
{
	if (Combat && Combat->EquippedWeapon == nullptr) return;

	if (IsAiming())
	{
		RotateCharacterInPlace(DeltaTime);
		if (bUseControllerRotationYaw == false) return;
		bUseControllerRotationYaw = false;
	}

	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bIsInAir = GetCharacterMovement()->IsFalling();
	AO_Pitch = GetBaseAimRotation().Pitch;

	if (Speed == 0.f && !bIsInAir)
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = FMath::Clamp(DeltaAimRotation.Yaw, -180.f, 180.f);
	}

	if (Speed > 0.f || bIsInAir) 
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		RotateStatementEnum = ERotateStatement::ERS_None;

		if (IsAiming())
		{
			if (bUseControllerRotationYaw == false)
			bUseControllerRotationYaw = true;
		}
		else
		{
			if (bUseControllerRotationYaw == true)
			bUseControllerRotationYaw = false;
		}
	}

	if (AO_Pitch > 90.f)
	{
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}


void ABaseCharacter::RotateCharacterInPlace(float DeltaTime)
{

	//if (AO_Yaw >= 90.f)
	//{
	//	RotateStatementEnum = ERotateStatement::ERS_Right;
	//}
	//else if (AO_Yaw <= -90.f)
	//{
	//	RotateStatementEnum = ERotateStatement::ERS_Left;
	//}

	//FRotator ControlRotation = GetControlRotation();
	//FRotator ActorRotation = GetActorRotation();

	//FRotator DeltaRotation = ControlRotation - ActorRotation;
	//DeltaRotation.Normalize();
}

bool ABaseCharacter::IsAiming()
{
	return (Combat && Combat->bAiming);
}

FVector ABaseCharacter::GetHitTarget() const
{
	if (Combat == nullptr) return FVector();
	return Combat->HitTarget;
}

bool ABaseCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

AWeapon* ABaseCharacter::GetEquippedWeapon()
{
	if (Combat == nullptr) return nullptr;
	return Combat->EquippedWeapon;
}

void ABaseCharacter::SetCurrentCharacterState(EWeaponEquippedStatus NewCharacterState)
{
	CurrentCharacterState = NewCharacterState;
}

EWeaponEquippedStatus ABaseCharacter::GetCurrentCharacterState() const
{
	return CurrentCharacterState;
}