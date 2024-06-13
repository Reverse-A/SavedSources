// ©2023 Reverse - A Company. All rights reserved.


#include "Character/BaseCharacterMovementComponent.h"

#include "Animation/AnimMontage.h"
#include "Animation/BaseAnimInstance.h"
#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"

#include "DrawDebugHelpers.h"

#if 1
float MacroDuration = 2.0f;
#define ServerLOG(x) GEngine->AddOnScreenDebugMessage(-1, MacroDuration ? MacroDuration : -1.0f, FColor::Cyan, x);
#define POINT(Position, Color) DrawDebugPoint(GetWorld(), Position, 10, Color, !MacroDuration, MacroDuration);
#define LINE(StartLoc, EndLoc, Color) DrawDebugLine(GetWorld(), StartLoc, EndLoc, Color, !MacroDuration, MacroDuration);
#define CAPSULE(CenterLoc, Color) DrawDebugCapsule(GetWorld(), CenterLoc, CapHalfHeight(), CapRadius(), FQuat::Identity, Color, !MacroDuration, MacroDuration);
#else
#define SLOG(x)
#define POINT(x, c)
#define LINE(x1, x2, c)
#define CAPSULE(x, c)
#endif


#pragma region Class Constructor

UBaseCharacterMovementComponent::UBaseCharacterMovementComponent()
{
	// The line of code enables the character's ability to crouch during gameplay.
	NavAgentProps.bCanCrouch = true;
}

void UBaseCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	BaseCharacter = Cast<ABaseCharacter>(GetOwner());
	AnimInstance = BaseCharacter->GetHeadMesh()->GetAnimInstance();

}
#pragma endregion Constructors

#pragma region Functions

#pragma endregion

#pragma region CustomMovementComp
bool UBaseCharacterMovementComponent::FSavedMove_Synthetica::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_Synthetica* NewSyntheticaMove = static_cast<FSavedMove_Synthetica*>(NewMove.Get());


	if (Saved_bWantsToSprint  != NewSyntheticaMove->Saved_bWantsToSprint)
	{
		return false;
	}
	if (Saved_bWantsToCrouch  != NewSyntheticaMove->Saved_bWantsToCrouch)
	{
		return false;
	}
	if (Saved_bWantsToSlide   != NewSyntheticaMove->Saved_bWantsToSlide)
	{
		return false;
	}
	if (Saved_bWantsToProne   != NewSyntheticaMove->Saved_bWantsToProne)
	{
		return false;
	}
	if (Saved_bWantsToDash	  != NewSyntheticaMove->Saved_bWantsToDash)
	{
		return false;
	}
	if (Saved_bWantsToWallRun != NewSyntheticaMove->Saved_bWantsToWallRun)
	{
		return false;
	}
	if (Saved_bWallRunIsRight != NewSyntheticaMove->Saved_bWallRunIsRight)
	{
		return false;
	}
	if (Saved_bWantsToHang	  != NewSyntheticaMove->Saved_bWantsToHang)
	{
		return false;
	}
	if (Saved_bWantsToClimb   != NewSyntheticaMove->Saved_bWantsToClimb)
	{
		return false;
	}
	

	return FSavedMove_Synthetica::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UBaseCharacterMovementComponent::FSavedMove_Synthetica::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint		= 0;
	Saved_bWantsToCrouch		= 0;
	Saved_bWantsToSlide			= 0;
	Saved_bWantsToProne			= 0;
	Saved_bWantsToDash			= 0;
	Saved_bWantsToWallRun		= 0;
	Saved_bWallRunIsRight		= 0;
	Saved_bWantsToHang			= 0;
	Saved_bWantsToClimb			= 0;

	Saved_bCustomJumpPressed	= 0;
	
	Saved_bHadAnimRootMotion	= 0;
	Saved_bTransitionFinished	= 0;
	
	
}

uint8 UBaseCharacterMovementComponent::FSavedMove_Synthetica::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if (Saved_bWantsToSprint) 		Result |= FLAG_Sprint; 
	if (Saved_bWantsToCrouch)		Result |= FLAG_Crouch;
	if (Saved_bWantsToSlide)		Result |= FLAG_Slide;
	if (Saved_bWantsToProne)		Result |= FLAG_Prone;
	if (Saved_bWantsToDash)			Result |= FLAG_Dash;
	if (Saved_bWantsToWallRun)		Result |= FLAG_WallRun;
	if (Saved_bWantsToHang)			Result |= FLAG_Hang;
	if (Saved_bWantsToClimb)		Result |= FLAG_Climb;
	if (Saved_bCustomJumpPressed)	Result |= FLAG_JumpPressed;

	return Result;
}
void UBaseCharacterMovementComponent::FSavedMove_Synthetica::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	const UBaseCharacterMovementComponent* BaseCharacterMovementComponent = Cast<UBaseCharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint		= BaseCharacterMovementComponent->Safe_bWantsToSprint;
	Saved_bWantsToCrouch		= BaseCharacterMovementComponent->Safe_bWantsToCrouch;
	Saved_bWantsToSlide			= BaseCharacterMovementComponent->Safe_bWantsToSlide;
	Saved_bWantsToProne			= BaseCharacterMovementComponent->Safe_bWantsToProne;
	Saved_bWantsToDash			= BaseCharacterMovementComponent->Safe_bWantsToDash;

	Saved_bWallRunIsRight		= BaseCharacterMovementComponent->Safe_bWallRunIsRight;


	Saved_bCustomJumpPressed	= BaseCharacterMovementComponent->BaseCharacter->bCustomJumpPressed;

	Saved_bHadAnimRootMotion	= BaseCharacterMovementComponent->Safe_bHadAnimRootMotion;
	Saved_bTransitionFinished	= BaseCharacterMovementComponent->Safe_bTransitionFinished;
}

void UBaseCharacterMovementComponent::FSavedMove_Synthetica::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);
	UBaseCharacterMovementComponent* BaseCharacterMovementComponent = Cast<UBaseCharacterMovementComponent>(C->GetCharacterMovement());

	BaseCharacterMovementComponent->Safe_bWantsToSprint					= Saved_bWantsToSprint;
	BaseCharacterMovementComponent->Safe_bWantsToCrouch					= Saved_bWantsToCrouch;
	BaseCharacterMovementComponent->Safe_bWantsToSlide					= Saved_bWantsToSlide;
	BaseCharacterMovementComponent->Safe_bWantsToProne					= Saved_bWantsToProne;

	BaseCharacterMovementComponent->Safe_bWallRunIsRight				= Saved_bWallRunIsRight;

	BaseCharacterMovementComponent->BaseCharacter->bCustomJumpPressed	= Saved_bCustomJumpPressed;

	BaseCharacterMovementComponent->Safe_bHadAnimRootMotion				= Saved_bHadAnimRootMotion;
	BaseCharacterMovementComponent->Safe_bTransitionFinished			= Saved_bTransitionFinished;

}

UBaseCharacterMovementComponent::FNetworkPredictionData_Client_Synthetica::FNetworkPredictionData_Client_Synthetica(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr UBaseCharacterMovementComponent::FNetworkPredictionData_Client_Synthetica::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Synthetica());
}

FNetworkPredictionData_Client* UBaseCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if (ClientPredictionData == nullptr)
	{
		UBaseCharacterMovementComponent* MutableThis = const_cast<UBaseCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Synthetica(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist  = 140.f;
	}

	return ClientPredictionData;
}

bool UBaseCharacterMovementComponent::IsServer() const
{
	return CharacterOwner->HasAuthority();
}

void UBaseCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Synthetica::FLAG_Sprint)  != 0;
	Safe_bWantsToCrouch = (Flags & FSavedMove_Synthetica::FLAG_Crouch)  != 0;
	Safe_bWantsToSlide	= (Flags & FSavedMove_Synthetica::FLAG_Slide)   != 0;
	Safe_bWantsToProne  = (Flags & FSavedMove_Synthetica::FLAG_Prone)   != 0;
	Safe_bWantsToDash	= (Flags & FSavedMove_Synthetica::FLAG_Dash)	!= 0;

}

void UBaseCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);


	DebugLog_CustomAndDefaultMovementState();

	if (IsMovementMode(MOVE_Flying))
	{
		if (!HasRootMotionSources())
		{
			SetMovementMode(MOVE_Walking);
		}
	}

	CurrentVelocitySize = Velocity.Size();

	if (IsMovementMode(MOVE_Falling))
	{
		if (IsCrouchingCustom())
		{
			StopCrouch();
		}

		if (IsSliding())
		{
			ExitSlide();
		}
	}


	
	if (IsMovementMode(MOVE_Walking))
	{
		// TODO : Gradually adjust acceleration at the startup && deceleration at the end.
		auto ControllerCrouchIsActivated = [&]() { return Safe_bWantsToCrouch; };
		auto ControllerSprintIsActivated = [&]() { return Safe_bWantsToSprint; };
		auto ControllerProneIsActivated  = [&]() { return Safe_bWantsToProne;  };

		if (ControllerCrouchIsActivated())
		{
			StartCrouch();
		}
		else
		{
			StopCrouch();
		}

		if (ControllerSprintIsActivated())
		{
			StartSprinting();
		}
		else
		{
			StopSprinting();
		}
		MaxWalkSpeed = CurrentMaxSpeed;
	}
}

void UBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);


	/*Exiting*/
	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Slide)
	{
		ExitSlide();
	}

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Prone)
	{
		ExitProne();
	}


	/*Entering*/
	if (IsCustomMovementMode(CMOVE_Slide))
	{
		EnterSlide(PreviousMovementMode, (ECustomMovementMode)PreviousCustomMode);
	}

	if (IsCustomMovementMode(CMOVE_Prone))
	{
		EnterProne(PreviousMovementMode, (ECustomMovementMode)PreviousCustomMode);
	}


	if (IsWallRunning() && GetOwnerRole() == ROLE_SimulatedProxy)
	{
		FVector Start = UpdatedComponent->GetComponentLocation();
		FVector End = Start + UpdatedComponent->GetRightVector() * CapRadius() * 2;
		auto CharacterIgnoredParameters = BaseCharacter->GetIgnoreCharacterParams();
		FHitResult WallHit;
		Safe_bWallRunIsRight = GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", CharacterIgnoredParameters);
	}
}

void UBaseCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (MovementMode == MOVE_Walking)
	{
		if (ControllerWantsToSlide())
		{
			SetMovementMode(MOVE_Custom, CMOVE_Slide);
		}

		if (ControllerWantsToProne())
		{
			SetMovementMode(MOVE_Custom, CMOVE_Prone);
			if (!CharacterOwner->HasAuthority())
			{
				Server_EnterProne();
			}
		}
	}

	if (IsCustomMovementMode(CMOVE_Prone) && !ControllerWantsToProne())
	{
		SetMovementMode(MOVE_Walking);
	}

	if (IsCustomMovementMode(CMOVE_Slide) && !ControllerWantsToSlide())
	{
		// Wants to cancel sliding by controller input
		bool FirstVelocityThreshold		= CurrentVelocitySize >= 300.0f && CurrentVelocitySize < 350.0f;
		bool SecondVelocityThreshold	= CurrentVelocitySize >= 350.0f && CurrentVelocitySize < 400.0f;
		bool ThirdVelocityThreshold		= CurrentVelocitySize >= 400.0f && CurrentVelocitySize < 450.0f;
		bool RestVelocityThreshold		= CurrentVelocitySize >= 450.0f && CurrentVelocitySize < 500.0f;
		bool MaxVelocityThreshold		= CurrentVelocitySize >= 500.0f;

		CanCancelSlideButtonTimer += DeltaSeconds;

		if (FirstVelocityThreshold)
		{
			CanCancelSlideButtonDelay	= 0.75f;
		}
		else if (SecondVelocityThreshold)
		{
			CanCancelSlideButtonDelay	= 1.25f;
		}
		else if (ThirdVelocityThreshold)
		{
			CanCancelSlideButtonDelay	= 1.75f;
		}
		else if (RestVelocityThreshold)
		{
			CanCancelSlideButtonDelay	= 2.5;
		}
		else if (MaxVelocityThreshold)
		{
			CanCancelSlideButtonDelay	= 3.0f;
		}
		else
		{
			CanCancelSlideButtonDelay	= ZERO;
		}

		if (CanCancelSlideButtonTimer > CanCancelSlideButtonDelay)
		{
			CanCancelSlideButtonTimer	= ZERO;
			ExitSlide();
		}
	}

	bool bAuthProxy = CharacterOwner->HasAuthority() && !CharacterOwner->IsLocallyControlled();
	if (ControllerWantsToDash() && CanDash())
	{
		if (!bAuthProxy || GetWorld()->GetTimeSeconds() - DashStartTime > AuthDashCooldownDuration)
		{
			PerformDash();
			Safe_bWantsToDash = false;
			Proxy_bDashStart = !Proxy_bDashStart;
		}
		else
		{	
			ServerLOG("Client tried to cheat");
		}
	}

	// Try Mantle
	if (BaseCharacter->bCustomJumpPressed)
	{
		if (TryMantle())
		{
			BaseCharacter->StopJumping();
		}
		else
		{
			BaseCharacter->bCustomJumpPressed	= false;
			CharacterOwner->bPressedJump		= true;
			CharacterOwner->CheckJumpInput(DeltaSeconds);
			bOrientRotationToMovement = true;
		}
	}

	// Transition Mantle
	if (Safe_bTransitionFinished)
	{	
		if (TransitionName == "Mantle" && AnimInstance)
		{
			if (IsValid(TransitionQueuedMontage))
			{
				SetMovementMode(MOVE_Flying);
				AnimInstance->Montage_Play(TransitionQueuedMontage, TransitionQueuedMontageSpeed);
				TransitionQueuedMontageSpeed = 0.f;
				TransitionQueuedMontage = nullptr;
			}
			else
			{
				SetMovementMode(MOVE_Walking);
			}
		}
		else if (TransitionName == "Hang")
		{
			LOG_WARNING("TODO : Hand")
		}
		TransitionName = "";
		Safe_bTransitionFinished = false;
	}

	if (IsFalling())
	{
		TryWallRun();
	}

	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UBaseCharacterMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

	if (!HasAnimRootMotion() && Safe_bHadAnimRootMotion && IsMovementMode(MOVE_Flying))
	{
		SetMovementMode(MOVE_Walking);
	}

	if (GetRootMotionSourceByID(TransitionRMS_ID) && GetRootMotionSourceByID(TransitionRMS_ID)->Status.HasFlag(ERootMotionSourceStatusFlags::Finished))
	{
		RemoveRootMotionSourceByID(TransitionRMS_ID);
		Safe_bTransitionFinished = true;
	}

	Safe_bHadAnimRootMotion = HasAnimRootMotion();
}

void UBaseCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		PhysSlide(deltaTime, Iterations);
		break;

	case CMOVE_Prone:
		PhysProne(deltaTime, Iterations);
		break;

	case CMOVE_WallRun:
		PhysWallRun(deltaTime, Iterations);
		break;

	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"));
	}
}

bool UBaseCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}
bool UBaseCharacterMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return InMovementMode == MovementMode;
}

bool UBaseCharacterMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide) || IsCustomMovementMode(CMOVE_Prone);
}

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	if (IsMovementMode(MOVE_Walking) && !IsCrouching() && IsControllerWantSprint())
	{
		return Sprint_MaxWalkSpeed;
	}

	if (MovementMode != MOVE_Custom)
	{
		return Super::GetMaxSpeed();
	}

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return Slide_MaxSpeed;

	case CMOVE_Prone:
		return Prone_MaxSpeed;

	case CMOVE_WallRun:
		return WallRun_MaxWalkSpeed;

	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
}

float UBaseCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode != MOVE_Custom)
	{
		return Super::GetMaxBrakingDeceleration();
	}

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return Slide_BrakingDeceleration;

	case CMOVE_Prone:
		return Prone_BrakingDeceleration;
	case CMOVE_WallRun:
		return ZERO;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
}

float UBaseCharacterMovementComponent::CapRadius() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
}
float UBaseCharacterMovementComponent::CapHalfHeight() const
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}
#pragma endregion CustomMovementComp


#pragma region		| Inputs - General |

#pragma region  | Jump	 System |
bool UBaseCharacterMovementComponent::CanAttemptJump() const
{
	return Super::CanAttemptJump() || IsWallRunning();
}

bool UBaseCharacterMovementComponent::DoJump(bool bReplayingMoves)
{
	bool bWasWallRunning = IsWallRunning();

	if (Super::DoJump(bReplayingMoves))
	{
		if (bWasWallRunning)
		{
			FVector Start		= UpdatedComponent->GetComponentLocation();
			FVector CastDelta	= UpdatedComponent->GetRightVector() * CapRadius() * 2;
			FVector End			= Safe_bWallRunIsRight ? Start + CastDelta : Start - CastDelta;

			auto IgnoredCharacterParameters = BaseCharacter->GetIgnoreCharacterParams();

			FHitResult WallHit;
			
			GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", IgnoredCharacterParameters);
			Velocity += WallHit.Normal * WallJumpOffForce;
			float JumpForceOppositeToWall = 200.f;

			Safe_bWallRunIsRight ? Velocity.X += JumpForceOppositeToWall : Velocity.X += -JumpForceOppositeToWall;
		}
		return true;
	}
	return false;
}
#pragma endregion	| Jump   System |

#pragma region Sprint Section

bool UBaseCharacterMovementComponent::CanSprint()
{
	// Check if conditions allow sprinting.
	return
		Super::IsMovingOnGround()	&& 
		!IsCrouchingCustom()		&&
		!IsSliding()				&&
		!IsFalling();
}

bool UBaseCharacterMovementComponent::IsSprinting()
{
	// Check if the character is currently sprinting.
	return bIsSprinting;
}

bool UBaseCharacterMovementComponent::IsControllerWantSprint() const
{
	// Check if the sprint input is active on the controller.
	return Safe_bWantsToSprint;
}

void UBaseCharacterMovementComponent::SetSprintInput(bool Trigger)
{
	// Set the sprint input trigger.
	Safe_bWantsToSprint = Trigger;
}


void UBaseCharacterMovementComponent::StartSprinting()
{
	
	// Start the sprinting action if allowed.


	// If character is crouching, exit the function.
	if (IsCrouchingCustom() || IsCrouching() || IsFalling() || IsSliding() || IsFlying())
		return;

	// If not already sprinting, set the sprinting flag to true.
	if(bIsSprinting != true)
		bIsSprinting = true;


	// Speed adjustment factors.
	float ScalingFactor				= 5.0f * 0.25f; // A scaling factor for adjustments.
	float HalfFactor				= 2.0f;			// Half the factor for calculations.
	float NearlyStoppedThreshold	= ((Walk_MaxWalkSpeed / HalfFactor) / HalfFactor); // Minimum velocity threshold for nearly stopped.

	// Get the time interval since the last frame.
	float DeltaSeconds				= GetWorld()->GetDeltaSeconds();


	// Calculate the threshold speed for transitioning to sprinting.
	float SprintThreshold			= ((Walk_MaxWalkSpeed + Sprint_MaxWalkSpeed) / HalfFactor);

	// Factor for controlling the speed transition smoothness.
	float LerpingAlphaValue			= 2.0f;						 // Controls how quickly the speed changes.
	float ScaledLerpingAlpha		= LerpingAlphaValue * 0.75f; // Scaled value for more gradual speed changes.


	// Adjust the character's speed based on the current state.
	if (CurrentMaxSpeed >= Walk_MaxWalkSpeed && CurrentMaxSpeed <= SprintThreshold)
	{
		// Gradually increase the speed towards the sprint threshold.
		CurrentMaxSpeed = FMath::Lerp(CurrentMaxSpeed, SprintThreshold + 100.0f, DeltaSeconds / LerpingAlphaValue);
	}
	else if (CurrentMaxSpeed >= SprintThreshold)
	{
		// Gradually increase the speed towards the sprinting maximum speed.
		CurrentMaxSpeed = FMath::Lerp(CurrentMaxSpeed, Sprint_MaxWalkSpeed, DeltaSeconds / LerpingAlphaValue * ScaledLerpingAlpha);
	}

	// Adjust the rotation rate for smoother turning while StartSprinting function is active.
	FRotator CharacterRotationRate = FRotator(0.0f, 250.0f, 0.0f);
	this->RotationRate = CharacterRotationRate;


	// Ensure the character maintains a minimum speed. If the controller didn't input any motion input value and the character suddenly experienced a speed drop, Set CurrentMaxSpeed Input Value to WalkMaxSpeed which is 300.f
	// In this way, a character who has made a sudden stop will not wander at sprint speeds again and will return to walking speed directly.
	if (CurrentVelocitySize < Walk_MaxWalkSpeed)
	{
		CurrentMaxSpeed	= Walk_MaxWalkSpeed;
	}
}

void UBaseCharacterMovementComponent::StopSprinting()
{
	// Stop the sprinting action if allowed.
	// If character is crouching, exit the function.
	if (IsCrouchingCustom()) return;


	// If the character is currently sprinting, set the sprinting flag to false to stop sprinting.
	if(bIsSprinting) bIsSprinting = false;


	// Speed adjustment factors. 
	float ScalingFactor = 5.0f * 0.25f; // A scaling factor for adjustments.
	float HalfFactor = 2.0f;			// Half the factor for calculations.
	float NearlyStoppedThreshold = ((Walk_MaxWalkSpeed / HalfFactor) / HalfFactor);  // Minimum velocity threshold for nearly stopped.

	// Get the time interval since the last frame.
	float DeltaSeconds = BaseCharacter->GetWorld()->GetDeltaSeconds();


	// Adjust the character's speed based on the current state and velocity.
	if (CurrentMaxSpeed > Walk_MaxWalkSpeed * ScalingFactor && CurrentVelocitySize > NearlyStoppedThreshold)
	{
		// Gradually decrease the speed towards normal walking speed.
		CurrentMaxSpeed = FMath::Lerp(CurrentMaxSpeed, Walk_MaxWalkSpeed, DeltaSeconds / 3.0f);
	}
	else if (CurrentVelocitySize < NearlyStoppedThreshold)
	{
		// If the character's velocity is very low, set the speed to normal walking speed.
		CurrentMaxSpeed = Walk_MaxWalkSpeed;
	}
	else
	{
		// If none of the above conditions apply, reset the speed to normal walking speed.
		CurrentMaxSpeed = Walk_MaxWalkSpeed;
	}

	// Adjust the rotation rate to facilitate smoother turning.
	this->RotationRate = FRotator(0.0f, 300.f, 0.0f);
}

#pragma endregion		| Sprint System |

#pragma region Crouch Section

/*Server Implementations |Crouch|*/

void UBaseCharacterMovementComponent::Server_EnterProne_Implementation()
{
	Safe_bWantsToProne = true;
}

bool UBaseCharacterMovementComponent::IsCrouchingCustom()
{
	return bIsCrouching;
}

bool UBaseCharacterMovementComponent::CanCrouch()
{
	return	IsMovingOnGround() && 
			!IsSprinting()	   && 
			!IsFalling()	   && 
			!IsSliding();
}

void UBaseCharacterMovementComponent::StartCrouch()
{
	if (IsSprinting()) return;

	if (bEnterCrouchBlockExecuted == false)
	{
		if (!bIsCrouching)
		{
			bIsCrouching = true;
		}

		if (CurrentMaxSpeed != Crouch_MaxWalkSpeed)
		{
			CurrentMaxSpeed = Crouch_MaxWalkSpeed;
		}

		USpringArmComponent* BaseCharSpringArm = BaseCharacter->GetBaseCharacterSpringArm();

		BaseCharSpringArm->bEnableCameraLag = true;
		BaseCharSpringArm->CameraLagSpeed = 25.f;

		BaseCharSpringArm->bEnableCameraRotationLag = true;
		BaseCharSpringArm->CameraRotationLagSpeed = 25.f;
		FRotator CrouchRotateRate = FRotator(0.0f, 100.f, 0.0f);

		RotationRate = CrouchRotateRate;
		CurrentMaxSpeed = Crouch_MaxWalkSpeed;

		if (bExitCrouchBlockExecuted)
		{
			bExitCrouchBlockExecuted = false;
		}
		bEnterCrouchBlockExecuted = true;
	}
}

void UBaseCharacterMovementComponent::StopCrouch()
{
	if (IsSprinting()) return;

	if (bIsCrouching)
	{
		if (bExitCrouchBlockExecuted == false)
		{
			bIsCrouching = false;
			float CapsuleHalfHeightNormal = 90.0f;
			USpringArmComponent* BaseCharSpringArm = BaseCharacter->GetBaseCharacterSpringArm();

			if (BaseCharSpringArm->bEnableCameraLag != false)
				BaseCharSpringArm->bEnableCameraLag  = false;

			if (BaseCharSpringArm->CameraLagSpeed != 10.f)
				BaseCharSpringArm->CameraLagSpeed  = 10.f;

			BaseCharSpringArm->bEnableCameraRotationLag = false;
			BaseCharSpringArm->CameraRotationLagSpeed   = 10.f;

			bEnterCrouchBlockExecuted = false;

			bExitCrouchBlockExecuted = true;
		}
	}
}

void UBaseCharacterMovementComponent::SetCrouchInput(bool Trigger)
{
	Safe_bWantsToCrouch = Trigger;
}
#pragma endregion		| Crouch System |

#pragma region Slide Section
void UBaseCharacterMovementComponent::EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode)
{
	bIsSliding = true;
	bOrientRotationToMovement = false;
	Velocity += Velocity.GetSafeNormal2D() * Slide_EnterImpulse;
	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);
}

void UBaseCharacterMovementComponent::ExitSlide()
{
	if (CharacterOwner->bPressedJump)
	{
		float JumpValue = JumpZVelocity * CurrentVelocitySize / 50.f;
		FVector JumpVector = FVector(0.f, 0.f, JumpValue);
		BaseCharacter->LaunchCharacter(JumpVector, false, false);

	}
	else
	{
		bIsSliding = false;
		Safe_bWantsToSlide = false;
		bOrientRotationToMovement = true;
		SetMovementMode(MOVE_Walking);
	}
}

void UBaseCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CanSlide())
	{
		ExitSlide();
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	bJustTeleported = false;
	bool bCheckedFall = false;
	bool bTriedLedgeMove = false;
	float remainingTime = deltaTime;

	while ((remainingTime >= MIN_TICK_TIME) &&
		(Iterations < MaxSimulationIterations) &&
		CharacterOwner &&
		(CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
	{

		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		// Save current values
		UPrimitiveComponent* const OldBase = GetMovementBase();
		const FVector PreviousBaseLocation = (OldBase != NULL) ? OldBase->GetComponentLocation() : FVector::ZeroVector;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FFindFloorResult OldFloor = CurrentFloor;

		// Ensure velocity is horizontal.
		MaintainHorizontalGroundVelocity();
		const FVector OldVelocity = Velocity;

		FVector SlopeForce = CurrentFloor.HitResult.Normal;
		SlopeForce.Z = 0.f;
		Velocity += SlopeForce * Slide_GravityForce * deltaTime;

		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector().GetSafeNormal2D());

		// Apply acceleration
		CalcVelocity(timeTick, GroundFriction * Slide_FrictionFactor, false, GetMaxBrakingDeceleration());

		// Compute move parameters
		const FVector MoveVelocity = Velocity;
		const FVector Delta = timeTick * MoveVelocity;
		const bool bZeroDelta = Delta.IsNearlyZero();
		FStepDownResult StepDownResult;

		bool bFloorWalkable = CurrentFloor.IsWalkableFloor();

		if (bZeroDelta)
		{
			remainingTime = 0.f;
		}
		else
		{
			// try to move forward
			MoveAlongFloor(MoveVelocity, timeTick, &StepDownResult);

			if (IsFalling())
			{
				// pawn decide to jump up
				const float DesiredDistance = Delta.Size();
				if (DesiredDistance > KINDA_SMALL_NUMBER)
				{
					const float ActualDistance = (UpdatedComponent->GetComponentLocation() - OldLocation).Size2D();
					remainingTime += timeTick * (1.f - FMath::Min(1.f, ActualDistance / DesiredDistance));
				}
				StartNewPhysics(remainingTime, Iterations);
				return;
			}
			else if (IsSwimming()) // if entered water
			{
				StartSwimming(OldLocation, OldVelocity, timeTick, remainingTime, Iterations);
				return;
			}
		}

		// Update Floor.
		// StepUp might have already done it for us.

		if (StepDownResult.bComputedFloor)
		{
			CurrentFloor = StepDownResult.FloorResult;
		}
		else
		{
			FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, bZeroDelta, NULL);
		}


		// Check for ledges here
		const bool bCheckLedges = !CanWalkOffLedges();
		if (bCheckLedges && !CurrentFloor.IsWalkableFloor())
		{
			// Calculate Possible alternate movement
			const FVector GravityDirection = FVector(0.0f, 0.0f, -1.0f);
			const FVector NewDelta = bTriedLedgeMove ? FVector::ZeroVector : GetLedgeMove(OldLocation, Delta, GravityDirection);
			if (!NewDelta.IsZero())
			{
				// First revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, false);

				// avoid repeated ledge moves if the first one fails
				bTriedLedgeMove = true;

				// Try new movement direction
				Velocity = NewDelta / timeTick;
				remainingTime += timeTick;
				continue;
			}
			else
			{
				// se if it is okay to jump
				// TODO: Collision : only thing that can be problem is that oldbase has world collision on
				bool bMustJump = bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
				{
					return;
				}
				bCheckedFall = true;

				// revert this move 
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
				remainingTime = 0.f;
				break;
			}
		}
		else
		{
			// Validate the floor check;
			if (CurrentFloor.IsWalkableFloor())
			{
				if (ShouldCatchAir(OldFloor, CurrentFloor))
				{
					HandleWalkingOffLedge(OldFloor.HitResult.ImpactNormal, OldFloor.HitResult.Normal, OldLocation, timeTick);

					if (IsMovingOnGround())
					{
						// If still walking, then fall. If not, assume the user set a different mode they want to keep.
						StartFalling(Iterations, remainingTime, timeTick, Delta, OldLocation);
					}
					return;
				}
				AdjustFloorHeight();
				SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
			}
			else if (CurrentFloor.HitResult.bStartPenetrating && remainingTime <= 0.0f)
			{
				// The floor check failed because it started in penetration
				// We do not want to try to move downward because the downward sweep failed, rather we'd like to try to pop out of the floor.
				FHitResult Hit(CurrentFloor.HitResult);
				Hit.TraceEnd = Hit.TraceStart + FVector(0.0f, 0.0f, MAX_FLOOR_DIST);
				const FVector RequestedAdjustment = GetPenetrationAdjustment(Hit);
				ResolvePenetration(RequestedAdjustment, Hit, UpdatedComponent->GetComponentQuat());
				bForceNextFloorCheck = true;
			}

			// Check if just entered water
			if (IsSwimming())
			{
				StartSwimming(OldLocation, Velocity, timeTick, remainingTime, Iterations);
				return;
			}

			// if we need to start falling
			if (!CurrentFloor.IsWalkableFloor() && !CurrentFloor.HitResult.bStartPenetrating)
			{
				const bool bMustJump = bJustTeleported || bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
				{
					return;
				}
				bCheckedFall = true;
			}
		}

		// Allow overlap events and such to change physics state and velocity
		if (IsMovingOnGround() && bFloorWalkable)
		{
			// Make velocity reflect actual move
			if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && timeTick >= MIN_TICK_TIME)
			{
				// TODO-RootMotionSource: Allow this to happen during partial override Velocity, but only set allowed axes?
				Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick;
				MaintainHorizontalGroundVelocity();
			}
		}

		// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
	}

	FHitResult Hit;
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(Velocity.GetSafeNormal2D(), FVector::UpVector).ToQuat();
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, false, Hit);
}

bool UBaseCharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start		= UpdatedComponent->GetComponentLocation();
	FVector End			= Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f * FVector::DownVector;
	FName ProfileName	= TEXT("BlockAll");

	return GetWorld()->LineTraceSingleByProfile(Hit, Start, End, ProfileName, BaseCharacter->GetIgnoreCharacterParams());
}

bool UBaseCharacterMovementComponent::CanSlide()
{
	FVector Start		= UpdatedComponent->GetComponentLocation();
	FVector End			= Start + CapHalfHeight() * 2.5f * FVector::DownVector;
	FName ProfileName	= TEXT("BlockAll");

	bool bValidSurface	= GetWorld()->LineTraceTestByProfile(Start, End, ProfileName, BaseCharacter->GetIgnoreCharacterParams());
	bool bEnoughSpeed	= Velocity.SizeSquared() > pow(Slide_MinSpeed, 2);

	return	bValidSurface && bEnoughSpeed;
}

bool UBaseCharacterMovementComponent::ShouldAllowSlideButtonPress()
{
	if (GetSafeWantsToSlide() && IsSliding())
	{
		bCanInitiateSlideCooldown = true;
	}

	bool FunctionReturner;

	if (bCanInitiateSlideCooldown)
	{
		if (!IsSliding())
		{
			CanPressSlideButtonTimer += GetWorld()->GetDeltaSeconds();

			if (CanPressSlideButtonTimer >= CanSlideAgainDelay)
			{
				// Timer has exceeded the delay, allow sliding again
				CanPressSlideButtonTimer = ZERO;
				bCanInitiateSlideCooldown = false;
			}
		}
		FunctionReturner = false;
	}
	else
	{
		FunctionReturner = true;
	}
	return FunctionReturner;
}

bool UBaseCharacterMovementComponent::ControllerWantsToSlide()
{
	return Safe_bWantsToSlide;
}

bool UBaseCharacterMovementComponent::IsSliding()
{
	return bIsSliding;
}


void UBaseCharacterMovementComponent::SetSlideInput(bool Trigger)
{
	Safe_bWantsToSlide = Trigger;
}

#pragma endregion		| Slide  System |

#pragma region |Prone|
void UBaseCharacterMovementComponent::EnterProne(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode)
{
	if (!bIsProne)
	{
		bIsProne = true;
	}

	if (PrevMode == MOVE_Custom && PrevCustomMode == CMOVE_Slide)
	{
		Velocity += Velocity.GetSafeNormal2D() * Prone_SlideEnterImpulse;
	}
	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);
}

void UBaseCharacterMovementComponent::ExitProne()
{
	if (IsProne())
	{
		bIsProne = false;
	}
}

void UBaseCharacterMovementComponent::PhysProne(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity	 = FVector::ZeroVector;
		return;
	}
	bJustTeleported		 = false;
	bool bCheckedFall    = false;
	bool bTriedLedgeMove = false;
	float remainingTime  = deltaTime;

	// Perform the move
	while ((remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
	{

		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		// Save current values
		UPrimitiveComponent* const OldBase = GetMovementBase();
		const FVector PreviousBaseLocation = (OldBase != NULL) ? OldBase->GetComponentLocation() : FVector::ZeroVector;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FFindFloorResult OldFloor = CurrentFloor;

		// Ensure velocity is horizontal.
		MaintainHorizontalGroundVelocity();
		const FVector OldVelocity = Velocity;
		Acceleration.Z = ZERO;

		// Apply acceleration
		CalcVelocity(timeTick, GroundFriction, false, GetMaxBrakingDeceleration());

		// Compute move parameters
		const FVector MoveVelocity = Velocity;
		const FVector Delta = timeTick * MoveVelocity; // dx = v * dt
		const bool bZeroDelta = Delta.IsNearlyZero();
		FStepDownResult StepDownResult;

		if (bZeroDelta)
		{
			remainingTime = ZERO;
		}
		else
		{
			MoveAlongFloor(MoveVelocity, timeTick, &StepDownResult);

			if (IsFalling())
			{
				// pawn decided to jump up
				const float DesiredDist = Delta.Size();
				if (DesiredDist > KINDA_SMALL_NUMBER)
				{
					const float ActualDist = (UpdatedComponent->GetComponentLocation() - OldLocation).Size2D();
					remainingTime += timeTick * (1.f - FMath::Min(1.f, ActualDist / DesiredDist));
				}

				StartNewPhysics(remainingTime, Iterations);
				return;
			}
			else if (IsSwimming()) //just entered water
			{
				StartSwimming(OldLocation, OldVelocity, timeTick, remainingTime, Iterations);
				return;
			}
		}

		// Update floor.
		// StepUp might have already done it for us.
		if (StepDownResult.bComputedFloor)
		{
			CurrentFloor = StepDownResult.FloorResult;
		}
		else
		{
			FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, bZeroDelta, NULL);
		}


		// check for ledges here
		const bool bCheckLedges = !CanWalkOffLedges();
		if (bCheckLedges && !CurrentFloor.IsWalkableFloor())
		{
			// calculate possible alternate movement
			const FVector GravityDirection = FVector(0.f, 0.f, -1.f);
			const FVector NewDelta = bTriedLedgeMove ? FVector::ZeroVector : GetLedgeMove(OldLocation, Delta, GravityDirection);
			if (!NewDelta.IsZero())
			{
				// first revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, false);

				// avoid repeated ledge moves if the first one fails
				bTriedLedgeMove = true;

				// Try new movement direction
				Velocity = NewDelta / timeTick; // v = dx/dt
				remainingTime += timeTick;
				continue;
			}
			else
			{
				// see if it is OK to jump
				// @todo collision : only thing that can be problem is that oldbase has world collision on
				bool bMustJump = bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
				{
					return;
				}
				bCheckedFall = true;

				// revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
				remainingTime = ZERO;
				break;
			}
		}
		else
		{
			// Validate the floor check
			if (CurrentFloor.IsWalkableFloor())
			{
				AdjustFloorHeight();
				SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
			}
			else if (CurrentFloor.HitResult.bStartPenetrating && remainingTime <= 0.f)
			{
				// The floor check failed because it started in penetration
				// We do not want to try to move downward because the downward sweep failed, rather we'd like to try to pop out of the floor.
				FHitResult Hit(CurrentFloor.HitResult);
				Hit.TraceEnd = Hit.TraceStart + FVector(0.f, 0.f, MAX_FLOOR_DIST);
				const FVector RequestedAdjustment = GetPenetrationAdjustment(Hit);
				ResolvePenetration(RequestedAdjustment, Hit, UpdatedComponent->GetComponentQuat());
				bForceNextFloorCheck = true;
			}

			// check if just entered water
			if (IsSwimming())
			{
				StartSwimming(OldLocation, Velocity, timeTick, remainingTime, Iterations);
				return;
			}

			// See if we need to start falling.
			if (!CurrentFloor.IsWalkableFloor() && !CurrentFloor.HitResult.bStartPenetrating)
			{
				const bool bMustJump = bJustTeleported || bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
				{
					return;
				}
				bCheckedFall = true;
			}
		}

		// Allow overlap events and such to change physics state and velocity
		if (IsMovingOnGround())
		{
			// Make velocity reflect actual move
			if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && timeTick >= MIN_TICK_TIME)
			{
				// TODO-RootMotionSource: Allow this to happen during partial override Velocity, but only set allowed axes?
				Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick; // v = dx / dt
				MaintainHorizontalGroundVelocity();
			}
		}

		// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = ZERO;
			break;
		}
	}

	if (IsMovingOnGround())
	{
		MaintainHorizontalGroundVelocity();
	}
}

bool UBaseCharacterMovementComponent::CanProne() const
{
	return IsCustomMovementMode(CMOVE_Slide) || IsMovementMode(MOVE_Walking) || IsCrouching() || IsCustomMovementMode(CMOVE_Prone);
}

bool UBaseCharacterMovementComponent::IsProne() const
{
	return bIsProne;
}

void UBaseCharacterMovementComponent::SetProneInput(bool Trigger)
{
	Safe_bWantsToProne = Trigger;
}

bool UBaseCharacterMovementComponent::ControllerWantsToProne()
{
	return Safe_bWantsToProne;
}

bool UBaseCharacterMovementComponent::IsProneButtonActive()
{
	if (bProneInputCooldownActive)
	{
		ProneButtonTimer += GetWorld()->GetDeltaSeconds();

		if (ProneButtonTimer >= PRoneButtonDelay)
		{
			ProneButtonTimer = 0.0f;
			bProneInputCooldownActive = false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

void UBaseCharacterMovementComponent::SetProneInputCooldown(bool bActive)
{
	bProneInputCooldownActive = bActive;
}
#pragma endregion				| Prone  System |

#pragma region |Dash|
bool UBaseCharacterMovementComponent::CanDash() const
{
	return IsWalking() && !IsCrouching() || IsFalling();
}

void UBaseCharacterMovementComponent::PerformDash()
{
	DashStartTime = GetWorld()->GetTimeSeconds();

	SetMovementMode(MOVE_Flying);

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(DashMontage);
	}
	DashStartDelegate.Broadcast();
}

void UBaseCharacterMovementComponent::OnDashCooldownFinished()
{
	Safe_bWantsToDash = true;
}

bool UBaseCharacterMovementComponent::ControllerWantsToDash()
{
	return Safe_bWantsToDash;
}

void UBaseCharacterMovementComponent::SetDashInput(bool bTrigger)
{
	Safe_bWantsToDash = bTrigger;
}
#pragma endregion				| Dash	 System |

#pragma region | Mantle |

bool UBaseCharacterMovementComponent::TryMantle()
{
	auto IgnoredCharacterParameters = BaseCharacter->GetIgnoreCharacterParams();

	if (!(IsMovementMode(MOVE_Walking) && !IsCrouching()) && !IsMovementMode(MOVE_Falling))
	{
		return false;
	}

	FVector BaseLocation  = UpdatedComponent->GetComponentLocation() + FVector::DownVector * CapHalfHeight();
	FVector ForwardVector = UpdatedComponent->GetForwardVector().GetSafeNormal2D();

	float MaxHeight = CapHalfHeight() * 2 + MantleReachHeight;
	float CosMMWSA  = FMath::Cos(FMath::DegreesToRadians(MantleMinWallSteepnessAngle));
	float CosMMSA   = FMath::Cos(FMath::DegreesToRadians(MantleMaxSurfaceAngle));
	float CosMMAA   = FMath::Cos(FMath::DegreesToRadians(MantleMaxAlignmentAngle));

	// Front Check
	FHitResult FrontHit;
	
	float CheckDistance = FMath::Clamp(Velocity | ForwardVector, CapRadius() + 30.0f, MantleMaxDistance);
	FVector FrontStart  = BaseLocation + FVector::UpVector * (MaxStepHeight - 1);

	for (int i = 0; i < 6; i++)
	{
		//LINE(FrontStart, FrontStart + ForwardVector * CheckDistance, FColor::Red);
		if (GetWorld()->LineTraceSingleByProfile(FrontHit, FrontStart, FrontStart + ForwardVector * CheckDistance, "BlockAll", IgnoredCharacterParameters))
		{
			break;
		}
		FrontStart += FVector::UpVector * (2.0f * CapHalfHeight() - (MaxStepHeight - 1)) / 5;
	}

	if (!FrontHit.IsValidBlockingHit())
	{
		return false;
	}

	float CosWallSteepnessAngle = FrontHit.Normal | FVector::UpVector;
	if (FMath::Abs(CosWallSteepnessAngle) > CosMMWSA || (ForwardVector | -FrontHit.Normal) < CosMMAA)
	{
		return false;
	}

	//Check Height
	TArray<FHitResult> HeightHits;
	FHitResult SurfaceHit;

	FVector WallUp   = FVector::VectorPlaneProject(FVector::UpVector, FrontHit.Normal).GetSafeNormal();
	float WallCosine = FVector::UpVector | FrontHit.Normal;
	float WallSine   = FMath::Sqrt(1 - WallCosine * WallCosine);

	FVector TraceStart = FrontHit.Location + ForwardVector + WallUp * (MaxHeight - (MaxStepHeight - 1)) / WallSine;
	//LINE(TraceStart, FrontHit.Location + ForwardVector, FColor::Orange);

	if (!GetWorld()->LineTraceMultiByProfile(HeightHits, TraceStart, FrontHit.Location + ForwardVector, "BlockAll", IgnoredCharacterParameters))
	{
		return false;
	}

	for (const FHitResult& Hit : HeightHits)
	{
		if (Hit.IsValidBlockingHit())
		{
			SurfaceHit = Hit;
			break;
		}
		if (SurfaceHit.IsValidBlockingHit())
		{
		}
	}

	if (!SurfaceHit.IsValidBlockingHit() || (SurfaceHit.Normal | FVector::UpVector) < CosMMSA)
	{
		return false;
	}
	float Height = (SurfaceHit.Location - BaseLocation) | FVector::UpVector;

	if (Height > MaxHeight)
	{
		return false;
	}

	// Check Clearance
	float SurfaceCosine			 = FVector::UpVector | SurfaceHit.Normal;
	float SurfaceSine			 = FMath::Sqrt(1 - SurfaceCosine * SurfaceCosine);

	FVector ClearCapsuleLocation = SurfaceHit.Location + ForwardVector * CapRadius() + FVector::UpVector * (CapHalfHeight() + 1 + CapRadius() * 2 * SurfaceSine);
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapRadius(), CapHalfHeight());

	if (GetWorld()->OverlapAnyTestByProfile(ClearCapsuleLocation, FQuat::Identity, "BlockAll", CapsuleShape, IgnoredCharacterParameters))
	{
		CAPSULE(ClearCapsuleLocation, FColor::Red);
		return false;
	}
	else
	{
		CAPSULE(ClearCapsuleLocation, FColor::Green);
	}
	

	// Mantle Selection
	FVector ShortMantleTarget	= GetMantleStartLocation(FrontHit, SurfaceHit, false);
	FVector TallMantleTarget	= GetMantleStartLocation(FrontHit, SurfaceHit, true);

	bool bTallMantle = false;

	if (IsMovementMode(MOVE_Walking) && Height > CapHalfHeight() * 2)
	{
		bTallMantle = true;
	}
	else if (IsMovementMode(MOVE_Falling) && (Velocity | FVector::UpVector) < 0)
	{
		if (!GetWorld()->OverlapAnyTestByProfile(TallMantleTarget, FQuat::Identity, "BlockAll", CapsuleShape, IgnoredCharacterParameters))
		{
			bTallMantle = true;
		}
	}

	FVector TransitionTarget = bTallMantle ? TallMantleTarget : ShortMantleTarget;

	CAPSULE(TransitionTarget, FColor::Yellow);
	// Perform Transition to Mantle
	CAPSULE(UpdatedComponent->GetComponentLocation(), FColor::Red);


	float UpSpeed			 = Velocity | FVector::UpVector;
	float TransitionDistance = FVector::Dist(TransitionTarget, UpdatedComponent->GetComponentLocation());


	TransitionQueuedMontageSpeed = FMath::GetMappedRangeValueClamped(FVector2D(-500.0f, 750.0f), FVector2D(.9f, 1.2f), UpSpeed);
	TransitionRMS.Reset();
	TransitionRMS = MakeShared<FRootMotionSource_MoveToForce>();
	TransitionRMS->AccumulateMode = ERootMotionAccumulateMode::Override;

	TransitionRMS->Duration = FMath::Clamp(TransitionDistance / 500.f, .1f, .25f);

	TransitionRMS->StartLocation  = UpdatedComponent->GetComponentLocation();
	TransitionRMS->TargetLocation = TransitionTarget;

	// Apply Transition Root Motion Source
	Velocity = FVector::ZeroVector;
	SetMovementMode(MOVE_Flying);

	TransitionRMS_ID = ApplyRootMotionSource(TransitionRMS);
	TransitionName = "Mantle";

	if (bTallMantle)
	{
		//BaseCharacter->PlayAnimMontage(TransitionTallMantleMontage, 1 / TransitionRMS->Duration);
		//CharacterOwner->PlayAnimMontage(TransitionTallMantleMontage, 1 / TransitionRMS->Duration);

		TransitionQueuedMontage = TallMantleMontage;
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(TransitionTallMantleMontage, 1 / TransitionRMS->Duration);
		}

		if (IsServer())
		{
			Proxy_bTallMantle = !Proxy_bTallMantle;
		}
	}
	else
	{
		//CharacterOwner->PlayAnimMontage(TransitionShortMantleMontage, 1 / TransitionRMS->Duration);

		TransitionQueuedMontage = ShortMantleMontage;

		if (AnimInstance)
		{
			AnimInstance->Montage_Play(TransitionShortMantleMontage, 1 / TransitionRMS->Duration);
		}

		if (IsServer())
		{
			Proxy_bShortMantle = !Proxy_bShortMantle;
		}
	}

	return true;
}

FVector UBaseCharacterMovementComponent::GetMantleStartLocation(FHitResult FrontHit, FHitResult SurfaceHit, bool bTallMantle) const
{
	float CosWallSteepnessAngle = FrontHit.Normal | FVector::UpVector;
	float DownDistance			= bTallMantle ? CapHalfHeight() * 2.f : MaxStepHeight - 1;

	FVector EdgeTangent			= FVector::CrossProduct(SurfaceHit.Normal, FrontHit.Normal).GetSafeNormal();
	FVector MantleStart			= SurfaceHit.Location;

	MantleStart				   += FrontHit.Normal.GetSafeNormal2D() * (2.f + CapRadius());
	MantleStart				   += UpdatedComponent->GetForwardVector().GetSafeNormal2D().ProjectOnTo(EdgeTangent) * CapRadius() * .3f;
	MantleStart				   += FVector::UpVector * CapHalfHeight();
	MantleStart				   += FVector::DownVector * DownDistance;
	MantleStart				   += FrontHit.Normal.GetSafeNormal2D() * CosWallSteepnessAngle * DownDistance;

	return MantleStart;
}

#pragma endregion			| Mantle System |

#pragma region		| WallRun-System|
bool UBaseCharacterMovementComponent::TryWallRun()
{
	if (!IsFalling())
	{
		return false;
	}
	if (Velocity.SizeSquared2D() < pow(WallRun_MinWalkSpeed, 2))
	{
		return false;
	}
	if (Velocity.Z < -MaxVerticalWallRunSpeed)
	{
		return false;
	}

	FVector Start							= UpdatedComponent->GetComponentLocation();
	auto IgnoredCharacterParameters			= BaseCharacter->GetIgnoreCharacterParams();


	FHitResult
		FloorHit,
		WallHit;

	// Check Player Height
	FVector FloorCheckEndLocation			= Start + FVector::DownVector * (CapHalfHeight() + MinWallRunHeight);

	if (GetWorld()->LineTraceSingleByProfile(FloorHit, Start, FloorCheckEndLocation, "BlockAll", IgnoredCharacterParameters)) return false;

	// Left Cast
	FVector DownDirectionOffset				= FVector::DownVector * 50.0f;
	FVector WallRunCheck_StartLocation		= Start + DownDirectionOffset;
	FVector LeftWallDirectionEnd			= WallRunCheck_StartLocation - UpdatedComponent->GetRightVector() * CapRadius() * 2;
	FVector RightWallDirectionEnd			= WallRunCheck_StartLocation + UpdatedComponent->GetRightVector() * CapRadius() * 2;


	GetWorld()->LineTraceSingleByProfile
	(   WallHit,
		WallRunCheck_StartLocation,
		LeftWallDirectionEnd,
		"BlockAll",
		IgnoredCharacterParameters
	);

	if (WallHit.IsValidBlockingHit() && (Velocity | WallHit.Normal) < 0)
	{
		Safe_bWallRunIsRight = false;
	}
	else
	{ 
		GetWorld()->LineTraceSingleByProfile(WallHit, WallRunCheck_StartLocation, RightWallDirectionEnd, "BlockAll", IgnoredCharacterParameters);
		if (WallHit.IsValidBlockingHit() && (Velocity | WallHit.Normal) < 0)
		{
			Safe_bWallRunIsRight = true;
		}
		else
		{
			return false;
		}				
	}

	FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);
	if (ProjectedVelocity.SizeSquared2D() < pow(WallRun_MinWalkSpeed, 2))
	{
		return false;
	}

	Velocity = ProjectedVelocity;
	Velocity.Z = FMath::Clamp(Velocity.Z, 0.0f, MaxVerticalWallRunSpeed);
	SetMovementMode(MOVE_Custom, CMOVE_WallRun);

	return true;
}

void UBaseCharacterMovementComponent::PhysWallRun(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}

	bJustTeleported = false;
	float remainingTime = deltaTime;
	
	// Perform the move
	while ((remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
	{
		Iterations++;
		bJustTeleported				= false;
		const float timeTick		= GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime				-= timeTick;
		const FVector OldLocation	= UpdatedComponent->GetComponentLocation();

		FVector Start				= UpdatedComponent->GetComponentLocation();
		FVector CastDelta			= UpdatedComponent->GetRightVector() * CapRadius() * 2;
		FVector End					= Safe_bWallRunIsRight ? Start + CastDelta : Start - CastDelta;
		
		auto IgnoredCharacterParameters = BaseCharacter->GetIgnoreCharacterParams();
		float SinPullAwayAngle = FMath::Sin(FMath::DegreesToRadians(WallRunPullAwayAngle));

		FHitResult WallHit;
		GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", IgnoredCharacterParameters);

		bool bWantsToPullAway = 
			WallHit.IsValidBlockingHit() && 
			!Acceleration.IsNearlyZero() && 
			(Acceleration.GetSafeNormal() | WallHit.Normal) > SinPullAwayAngle;

		if (!WallHit.IsValidBlockingHit() || bWantsToPullAway)
		{
			LOG_WARNING("PROBLEM");
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(remainingTime, Iterations);
			return;
		}

		// Clamp Acceleration
		Acceleration = FVector::VectorPlaneProject(Acceleration, WallHit.Normal);
		Acceleration.Z = ZERO;

		//Apply acceleration
		CalcVelocity(timeTick, 0.0f, false, GetMaxBrakingDeceleration());
		Velocity = FVector::VectorPlaneProject(Velocity, WallHit.Normal);
		float TangentAccel = Acceleration.GetSafeNormal() | Velocity.GetSafeNormal2D();
		bool bVelocityUp = Velocity.Z > 0.f;


		if (IsWallRunning() && TangentAccel == 0.f)
		{
			SetMovementMode(MOVE_Falling);
		}

		Velocity.Z += GetGravityZ() * WallRunGravityScaleCurve->GetFloatValue(bVelocityUp ? 0.f : TangentAccel) * timeTick;
		if (Velocity.SizeSquared2D() < pow(WallRun_MinWalkSpeed, 2) || Velocity.Z < -MaxVerticalWallRunSpeed)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(remainingTime, Iterations);
			return;
		}

		// Compute move Parameters
		const FVector Delta = timeTick * Velocity; // dx = v * dt 
		const bool bZeroDelta = Delta.IsNearlyZero();
		if (bZeroDelta)
		{
			remainingTime = 0.0f;
		}
		else
		{
			FHitResult Hit;
			SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);
			FVector WallAttractionDelta = -WallHit.Normal * WallAttractionForce * timeTick;
			SafeMoveUpdatedComponent(WallAttractionDelta, UpdatedComponent->GetComponentQuat(), true, Hit);
		}

		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}	
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick; // v = dx / dt
	}

	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector CastDelta = UpdatedComponent->GetRightVector() * CapRadius() * 2;
	FVector End = Safe_bWallRunIsRight ? Start + CastDelta : Start - CastDelta;
	
	auto IgnoredCharacterParameters = BaseCharacter->GetIgnoreCharacterParams();

	FHitResult FloorHit, WallHit;
	GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", IgnoredCharacterParameters);
	GetWorld()->LineTraceSingleByProfile(FloorHit, Start, Start + FVector::DownVector * (CapHalfHeight() + MinWallRunHeight * .5f), "BlockAll", IgnoredCharacterParameters);
	if (FloorHit.IsValidBlockingHit() || !WallHit.IsValidBlockingHit() || Velocity.SizeSquared2D() < pow(WallRun_MinWalkSpeed, 2))
	{
		SetMovementMode(MOVE_Falling);
	}
}

#pragma endregion	| WallRun-System|

#pragma region |Replication|
void UBaseCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UBaseCharacterMovementComponent, Proxy_bDashStart,		COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UBaseCharacterMovementComponent, Proxy_bShortMantle,	COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UBaseCharacterMovementComponent, Proxy_bTallMantle,		COND_SkipOwner);

}
void UBaseCharacterMovementComponent::OnRep_DashStart()
{
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(DashMontage);
	}
	
	DashStartDelegate.Broadcast();
}

void UBaseCharacterMovementComponent::OnRep_ShortMantle()
{
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ProxyShortMantleMontage);
	}
	//CharacterOwner->PlayAnimMontage(ProxyShortMantleMontage);
}

void UBaseCharacterMovementComponent::OnRep_TallMantle()
{
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ProxyTallMantleMontage);
	}
	//CharacterOwner->PlayAnimMontage(ProxyTallMantleMontage);
}
#pragma endregion		| Replication   |

#pragma endregion	|Inputs - End Line |













void UBaseCharacterMovementComponent::DebugLog_CustomAndDefaultMovementState()
{
	FString CustomModeString;
	FString DefaultModeString;

	switch (CustomMovementMode)
	{
	case ECustomMovementMode::CMOVE_None:
		CustomModeString = TEXT("None");
		break;
	case ECustomMovementMode::CMOVE_Sprint:
		CustomModeString = TEXT("Sprint");
		break;
	case ECustomMovementMode::CMOVE_Crouch:
		CustomModeString = TEXT("Crouch");
		break;
	case ECustomMovementMode::CMOVE_Prone:
		CustomModeString = TEXT("Prone");
		break;
	case ECustomMovementMode::CMOVE_Slide:
		CustomModeString = TEXT("Slide");
		break;
	case ECustomMovementMode::CMOVE_Dash:
		CustomModeString = TEXT("Dash");
		break;
	case ECustomMovementMode::CMOVE_WallRun:
		CustomModeString = TEXT("Wall Run");
		break;
	case ECustomMovementMode::CMOVE_Hang:
		CustomModeString = TEXT("Hang");
		break;
	case ECustomMovementMode::CMOVE_Climb:
		CustomModeString = TEXT("Climb");
		break;
	case ECustomMovementMode::CMOVE_MAX:
		CustomModeString = TEXT("Max");
		break;
	default:
		CustomModeString = TEXT("Unknown");
		break;
	}

	switch (MovementMode)
	{
	case EMovementMode::MOVE_None:
		DefaultModeString = TEXT("None");
		break;
	case EMovementMode::MOVE_Walking:
		DefaultModeString = TEXT("Walking");
		break;
	case EMovementMode::MOVE_NavWalking:
		DefaultModeString = TEXT("Navmesh Walking");
		break;
	case EMovementMode::MOVE_Falling:
		DefaultModeString = TEXT("Falling");
		break;
	case EMovementMode::MOVE_Swimming:
		DefaultModeString = TEXT("Swimming");
		break;
	case EMovementMode::MOVE_Flying:
		DefaultModeString = TEXT("Flying");
		break;
	case EMovementMode::MOVE_Custom:
		DefaultModeString = TEXT("Custom");
		break;
	case EMovementMode::MOVE_MAX:
		DefaultModeString = TEXT("Max");
		break;
	default:
		DefaultModeString = TEXT("Unknown");
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Custom Movement Mode: %s"), *CustomModeString);
	UE_LOG(LogTemp, Warning, TEXT("Default Movement Mode: %s"), *DefaultModeString);
}