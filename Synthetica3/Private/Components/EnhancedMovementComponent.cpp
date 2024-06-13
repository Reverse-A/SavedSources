// ©2023 Reverse - A Company. All rights reserved.


#include "Components/EnhancedMovementComponent.h"

#include "Net/UnrealNetwork.h"

#include "Camera/CameraSettings.h"
#include "Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



#pragma region || Class Constructor and bases || 
UEnhancedMovementComponent::UEnhancedMovementComponent()
{
	NavAgentProps.bCanCrouch = true;

	Synthetica_ServerMoveBitWriter.SetAllowResize(true);

	RotationRate = FRotator(0.0f, 0.0f, 250.0f);
	bIsSliding = false;
}

void UEnhancedMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickCount++;
	DebugEachClient();
	StoredDeltaTime = GetWorld()->GetDeltaSeconds();

	if (CurrentVelocitySize > 0.0f)
	{
		CalculateSlopeAngle(DeltaTime);
	}
}
#pragma endregion

#pragma region | Server Properties |

UEnhancedMovementComponent::FSavedMove_Synthetica::FSavedMove_Synthetica()
{
	Saved_bWantsToSprint	= 0;
	Saved_bWantsToProne		= 0;
	//Saved_bWantsToSlide	= 0;
}

bool UEnhancedMovementComponent::FSavedMove_Synthetica::CanCombineWith(const FSavedMovePtr & NewMove, ACharacter * InCharacter, float MaxDelta) const
{
	FSavedMove_Synthetica* NewSyntheticaMove = static_cast<FSavedMove_Synthetica*>(NewMove.Get());


	if (Saved_bWantsToSprint	!= NewSyntheticaMove->Saved_bWantsToSprint)
	{
		return false;
	}
	if (Saved_bWantsToCrouch	!= NewSyntheticaMove->Saved_bWantsToCrouch)
	{
		return false;
	}
	//if (Saved_bWantsToSlide		!= NewSyntheticaMove->Saved_bWantsToSlide)
	//{
	//	return false;
	//}
	if (Saved_bWantsToProne		!= NewSyntheticaMove->Saved_bWantsToProne)
	{
		return false;
	}
	if (Saved_bWantsToDash		!= NewSyntheticaMove->Saved_bWantsToDash)
	{
		return false;
	}
	if (Saved_bWantsToWallRun	!= NewSyntheticaMove->Saved_bWantsToWallRun)
	{
		return false;
	}
	if (Saved_bWallRunIsRight	!= NewSyntheticaMove->Saved_bWallRunIsRight)
	{
		return false;
	}
	if (Saved_bWantsToHang		!= NewSyntheticaMove->Saved_bWantsToHang)
	{
		return false;
	}
	if (Saved_bWantsToClimb		!= NewSyntheticaMove->Saved_bWantsToClimb)
	{
		return false;
	}

	return FSavedMove_Synthetica::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UEnhancedMovementComponent::FSavedMove_Synthetica::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint		= 0;
	Saved_bWantsToCrouch		= 0;
	//Saved_bWantsToSlide			= 0;
	Saved_bWantsToProne			= 0;
	Saved_bWantsToDash			= 0;
	Saved_bWantsToWallRun		= 0;
	Saved_bWallRunIsRight		= 0;
	Saved_bWantsToHang			= 0;
	Saved_bWantsToClimb			= 0;

	Saved_bCustomJumpPressed	= 0;

	Saved_bHadAnimRootMotion	= 0;
	Saved_bTransitionFinished   = 0;
}

uint8 UEnhancedMovementComponent::FSavedMove_Synthetica::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if (Saved_bWantsToSprint)		Result |= FLAG_Sprint;
	if (Saved_bWantsToCrouch)		Result |= FLAG_Crouch;
	//if (Saved_bWantsToSlide)		Result |= FLAG_Slide;
	if (Saved_bWantsToProne)		Result |= FLAG_Prone;
	if (Saved_bWantsToDash)			Result |= FLAG_Dash;
	if (Saved_bWantsToWallRun)		Result |= FLAG_WallRun;
	if (Saved_bWantsToHang)			Result |= FLAG_Hang;
	if (Saved_bWantsToClimb)		Result |= FLAG_Climb;
	if (Saved_bCustomJumpPressed)	Result |= FLAG_JumpPressed;

	return Result;
}

void UEnhancedMovementComponent::FSavedMove_Synthetica::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
	const UEnhancedMovementComponent* EnhancedMovementComponent = Cast<UEnhancedMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint		= EnhancedMovementComponent->Safe_bWantsToSprint;
	Saved_bWantsToProne			= EnhancedMovementComponent->Safe_bWantsToProne;

	Saved_bHadAnimRootMotion	= EnhancedMovementComponent->Safe_bHadAnimRootMotion;
	Saved_bTransitionFinished	= EnhancedMovementComponent->Safe_bTransitionFinished;

	Saved_bCustomJumpPressed	= EnhancedMovementComponent->BaseCharacter->bCustomJump;

	Saved_bWallRunIsRight		= EnhancedMovementComponent->Safe_bWallRunIsRight;

}

void UEnhancedMovementComponent::FSavedMove_Synthetica::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);
	UEnhancedMovementComponent* EnhancedMovementComponent = Cast<UEnhancedMovementComponent>(C->GetCharacterMovement());

	EnhancedMovementComponent->Safe_bWantsToSprint		= Saved_bWantsToSprint;
	EnhancedMovementComponent->Safe_bWantsToProne		= Saved_bWantsToProne;

	EnhancedMovementComponent->Safe_bHadAnimRootMotion  = Saved_bHadAnimRootMotion;
	EnhancedMovementComponent->Safe_bTransitionFinished = Saved_bTransitionFinished;

	EnhancedMovementComponent->Safe_bWallRunIsRight		= Saved_bWallRunIsRight;

	EnhancedMovementComponent->BaseCharacter->bCustomJump = Saved_bCustomJumpPressed;

}
#pragma endregion
#pragma region | Client Network Prediction Data |
UEnhancedMovementComponent::FNetworkPredictionData_Client_Synthetica::FNetworkPredictionData_Client_Synthetica(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr UEnhancedMovementComponent::FNetworkPredictionData_Client_Synthetica::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Synthetica());
}

#pragma endregion
#pragma region | EnhancedMovement (Class) |

void UEnhancedMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	BaseCharacter = Cast<ABaseCharacter>(GetOwner());
}

bool UEnhancedMovementComponent::IsMovingOnGround() const
{
	return  
		Super::IsMovingOnGround()			|| 
		IsCustomMovementMode(CMOVE_Slide)	||
		IsCustomMovementMode(CMOVE_Prone);
}

bool UEnhancedMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

float UEnhancedMovementComponent::GetMaxSpeed() const
{
	if (IsMovementMode(MOVE_Walking) && IsControllerWantToSprint() && !IsCrouching())
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
		return Prone_MaxWalkSpeed;
	case CMOVE_WallRun:
		return WallRun_MaxWalkSpeed;
	case CMOVE_Hang:
		return 0.0f;   
	case CMOVE_Climb:
		return Climb_MaxSpeed; // TODO: Climb  

	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
		return -1.f;
	}
}

float UEnhancedMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode != MOVE_Custom) return Super::GetMaxBrakingDeceleration();

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return Slide_BrakingDeceleration;
	case CMOVE_Prone:
		return Prone_BrakingDeceleration;
	case CMOVE_WallRun:
		return 0.0f; // TODO: WallRun
	case CMOVE_Hang:
		return 0.0f;  
	case CMOVE_Climb:
		return Climb_BrakingDeceleration; // TODO: Climb  

	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"));
		return -1.f;
	}
}


void UEnhancedMovementComponent::OnClientCorrectionReceived(FNetworkPredictionData_Client_Character& ClientData, float TimeStamp, FVector NewLocation, FVector NewVelocity, UPrimitiveComponent* NewBase, FName NewBaseBoneName, bool bHasBase, bool bBaseRelativePosition, uint8 ServerMovementMode, FVector ServerGravityDirection)
{
	Super::OnClientCorrectionReceived(ClientData, TimeStamp, NewLocation, NewVelocity, NewBase, NewBaseBoneName, bHasBase, bBaseRelativePosition, ServerMovementMode, ServerGravityDirection);

	CorrectionCount++;
}

float UEnhancedMovementComponent::CapRadius() const
{
	return BaseCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
}
float UEnhancedMovementComponent::CapHalfHeight() const
{
	return BaseCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void UEnhancedMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEnhancedMovementComponent, bIsSprinting);
	DOREPLIFETIME(UEnhancedMovementComponent, bIsProning);
	DOREPLIFETIME(UEnhancedMovementComponent, bIsSliding);
	DOREPLIFETIME(UEnhancedMovementComponent, bTallMantle);
	DOREPLIFETIME(UEnhancedMovementComponent, MantleTransitionTime);
	DOREPLIFETIME(UEnhancedMovementComponent, MantleTransitionDuration);
	DOREPLIFETIME(UEnhancedMovementComponent, OutSlopePitchDegreeAngle);

	DOREPLIFETIME_CONDITION(UEnhancedMovementComponent, Proxy_bShortMantle, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UEnhancedMovementComponent, Proxy_bTallMantle,  COND_SkipOwner);

}

FNetworkPredictionData_Client* UEnhancedMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if (ClientPredictionData == nullptr)
	{
		UEnhancedMovementComponent* MutableThis = const_cast<UEnhancedMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Synthetica(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	return ClientPredictionData;
}

void UEnhancedMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Synthetica::FLAG_Sprint) != 0;
}

void UEnhancedMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (MovementMode == EMovementMode::MOVE_Walking)
	{

		if (IsControllerWantToSprint() && !BaseCharacter->IsAiming())
		{
			IncreaseMovementSpeed_SprintMaxSpeed();
		}

		if (BaseCharacter->IsAiming())
		{
			if (IsControllerWantToSprint())
			{
				SetSprintInput(false);
			}
			if (MaxWalkSpeed != Aim_MaxWalkSpeed)
			{
				MaxWalkSpeed = Aim_MaxWalkSpeed;
			}
		}
		else
		{
			if (MaxWalkSpeed <= Aim_MaxWalkSpeed)
			{
				MaxWalkSpeed = Jog_MaxWalkSpeed;
			}
		}

		if (IsControllerWantsToProne() && !IsProning())
		{
			if (bIsProning != true)
			{
				bIsProning = true;
			}

			SetMovementMode(MOVE_Custom, CMOVE_Prone);
			if (CharacterOwner->HasAuthority())
			{
				Multicast_EnterProne();
			}
			else
			{
				Server_EnterProne();
			}
		}

		if (IsControllerWantsSlide() && !IsSliding())
		{
			SetMovementMode(MOVE_Custom, CMOVE_Slide);
		}
	}



	//if (IsHanging() && bWantsToCrouch)
	//{
	//	SetMovementMode(MOVE_Falling);
	//	bWantsToCrouch = false;
	//}
	//else if (IsFalling() && bWantsToCrouch)
	//{
	//	if (TryClimb())
	//	{
	//		bWantsToCrouch = false;
	//	}
	//}

	if (IsFalling())
	{
		TryWallRun();
	}


	if (BaseCharacter->bCustomJump)
	{
		if (TryMantle())
		{
			if (bCanMantle != true)
			{
				bCanMantle = true;
			}
			BaseCharacter->StopJumping();
		}
		else
		{
			BaseCharacter->bCustomJump = false;
			CharacterOwner->bPressedJump = true;
			CharacterOwner->CheckJumpInput(DeltaSeconds);
			bOrientRotationToMovement = true;
			bCanMantle = false;
		}
	}

	if (bCanMantle)
	{
		SetMantleLocation();
	}


	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}


void UEnhancedMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

	if (MovementMode == EMovementMode::MOVE_Walking)
	{
		if (!IsControllerWantToSprint() && MaxWalkSpeed > Jog_MaxWalkSpeed)
		{
			DecreaseMovementSpeed_AfterSprint();
		}
	}

	if (IsCustomMovementMode(CMOVE_Prone) && !IsControllerWantsToProne())
	{
		ExitProne();

		if (BaseCharacter->HasAuthority())
		{
			Multicast_ExitProne();
		}
		else
		{
			Server_ExitProne();
		}
	}

	if (IsCustomMovementMode(CMOVE_Slide) && !IsControllerWantsSlide())
	{
		if (!IsSliding()) return;

		// Wants to cancel sliding by controller input
		bool FirstVelocityThreshold  = CurrentVelocitySize >= 300.0f && CurrentVelocitySize < 350.0f;
		bool SecondVelocityThreshold = CurrentVelocitySize >= 350.0f && CurrentVelocitySize < 400.0f;
		bool ThirdVelocityThreshold  = CurrentVelocitySize >= 400.0f && CurrentVelocitySize < 450.0f;
		bool RestVelocityThreshold   = CurrentVelocitySize >= 450.0f && CurrentVelocitySize < 500.0f;
		bool MaxVelocityThreshold    = CurrentVelocitySize >= 500.0f;

		CanCancelSlideButtonTimer += DeltaSeconds;

		if (FirstVelocityThreshold)
		{
			CanCancelSlideButtonDelay = 0.75f;
		}
		else if (SecondVelocityThreshold)
		{
			CanCancelSlideButtonDelay = 1.25f;
		}
		else if (ThirdVelocityThreshold)
		{
			CanCancelSlideButtonDelay = 1.75f;
		}
		else if (RestVelocityThreshold)
		{
			CanCancelSlideButtonDelay = 2.5;
		}
		else if (MaxVelocityThreshold)
		{
			CanCancelSlideButtonDelay = 3.0f;
		}
		else
		{
			CanCancelSlideButtonDelay = ZERO;
		}

		if (CanCancelSlideButtonTimer > CanCancelSlideButtonDelay)
		{
			CanCancelSlideButtonTimer = ZERO;
			ExitSlide();
		}
	}

	Safe_bHadAnimRootMotion = HasAnimRootMotion();
}


void UEnhancedMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	CurrentVelocitySize = Velocity.Size();
	if (IsFalling() || IsMovingOnGround())
	{
		if (GravityResetTimer != 0.0f)
		{
			GravityResetTimer = 0.0f;
		}
	}

	if (BaseCharacter->IsAutoRun())
	{
		AutoRun();
	}

	//if (IsMovementMode(MOVE_Falling))
	//{
	//	if (IsCrouching())
	//	{
	//		UnCrouch();
	//	}

	//	if (IsSliding())
	//	{
	//		ExitSlide();
	//	}
	//}
}

void UEnhancedMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (!bEnterSlideExecution && IsCustomMovementMode(CMOVE_Slide))
	{
		EnterSlide(PreviousMovementMode, (ECustomMovementMode)PreviousCustomMode);
		bEnterSlideExecution = true;
		bExitSlideExecution = false;
	}

	if (!bExitSlideExecution && PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Slide)
	{
		ExitSlide();
		bExitSlideExecution = true;
		bEnterSlideExecution = false;
	}

	if (IsCustomMovementMode(CMOVE_Prone))
	{
		EnterProne(PreviousMovementMode, (ECustomMovementMode)PreviousCustomMode);
	}

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Prone)
	{
		ExitProne();
	}
}

void UEnhancedMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
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
	case CMOVE_Hang:
		break;
	//case CMOVE_Climb:
	//	PhysClimb(deltaTime, Iterations);
	//	break;
	}
}

bool UEnhancedMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

bool UEnhancedMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return InMovementMode == MovementMode;
}
#pragma endregion
#pragma region | Replication |

#pragma endregion
#pragma region | Different Functionalities |
void UEnhancedMovementComponent::AutoRun()
{
	float ControlRotationZ			= BaseCharacter->GetControlRotation().Roll;
	FVector ForwardVector			= BaseCharacter->GetActorForwardVector();
	FVector Direction				= FVector(ForwardVector.X, ForwardVector.Y, ControlRotationZ);
	FRotator AimRotation			= BaseCharacter->GetBaseAimRotation();
	FRotator InterpolatedRotation	= FMath::RInterpTo(BaseCharacter->GetActorRotation(), AimRotation, GetWorld()->GetDeltaSeconds(), 15.0f);

	BaseCharacter->SetActorRotation(InterpolatedRotation);
	BaseCharacter->AddMovementInput(Direction, 1, false);
}
#pragma endregion
#pragma region | Jump - System |
bool UEnhancedMovementComponent::CanAttemptJump() const
{
	return Super::CanAttemptJump() || IsWallRunning() || IsHanging() || IsClimbing();
}

bool UEnhancedMovementComponent::DoJump(bool bReplayingMoves)
{
	bool bWasWallRunning = IsWallRunning();
	bool bWasOnWall		 = IsHanging();

	if (Super::DoJump(bReplayingMoves))
	{
		if (bWasWallRunning)
		{
			FVector Start = UpdatedComponent->GetComponentLocation();
			FVector CastDelta = UpdatedComponent->GetRightVector() * CapRadius() * 2;
			FVector End = Safe_bWallRunIsRight ? Start + CastDelta : Start - CastDelta;

			auto IgnoredCharacterParameters = BaseCharacter->GetIgnoreCharacterParams();

			FHitResult WallHit;

			GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", IgnoredCharacterParameters);
			Velocity += WallHit.Normal * WallJumpOffForce;
			float JumpForceOppositeToWall = 200.f;

			Safe_bWallRunIsRight ? Velocity.X += JumpForceOppositeToWall : Velocity.X += -JumpForceOppositeToWall;
		}
		else if (bWasOnWall)
		{
			if (!bReplayingMoves)
			{
				BaseCharacter->PlayAnimMontage(WallJumpMontage);
			}
			Velocity += FVector::UpVector * WallJumpForce * 0.5f;
			Velocity += Acceleration.GetSafeNormal2D() * WallJumpForce * 0.5f;
		}
		return true;
	}
	return false;
}
#pragma endregion

#pragma region | Sprint |
bool UEnhancedMovementComponent::CanSprint()
{
	// Check if conditions allow sprinting.
	return
		Super::IsMovingOnGround() &&
		!IsCrouching() &&
		!IsFalling() &&
		!BaseCharacter->IsAiming();
}

void UEnhancedMovementComponent::IncreaseMovementSpeed_SprintMaxSpeed()
{
	if (IsCrouching() || IsFalling() || IsSliding() || IsFlying() || IsCustomMovementMode(CMOVE_Slide))
		return;

	if (CurrentVelocitySize < 400.0f)
	{
		SetSprintInput(false);
	}

	if (BaseCharacter->IsLocallyControlled())
	{
		Server_ActiveSprint();
	}

	if (MaxWalkSpeed == Sprint_MaxWalkSpeed)
	{
		return;
	}

	if (CurrentVelocitySize < Jog_MaxWalkSpeed)
	{
		CurrentMaxSpeed = Jog_MaxWalkSpeed;
	}
	// Speed adjustment factors.
	float ScalingFactor				= 5.0f * 0.25f; // A scaling factor for adjustments.
	float HalfFactor				= 2.0f;			// Half the factor for calculations.
	float NearlyStoppedThreshold	= ((Jog_MaxWalkSpeed / HalfFactor) / HalfFactor); // Minimum velocity threshold for nearly stopped.

	// Get the time interval since the last frame.
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();

	// Calculate the threshold speed for transitioning to sprinting.
	float SprintThreshold		= ((Jog_MaxWalkSpeed + Sprint_MaxWalkSpeed) / HalfFactor);
	float SprintMaxSpeedThreshold = 750.0f;

	// Factor for controlling the speed transition smoothness.
	float LerpingAlphaValue		= 2.0f;						 // Controls how quickly the speed changes.
	float ScaledLerpingAlpha	= LerpingAlphaValue * 0.75f; // Scaled value for more gradual speed changes.

	// Adjust the character's speed based on the current state.
	if (MaxWalkSpeed >= Jog_MaxWalkSpeed && MaxWalkSpeed < SprintThreshold)
	{
		// Gradually increase the speed towards the sprint threshold.
		MaxWalkSpeed = FMath::Lerp(MaxWalkSpeed, SprintThreshold + 100.0f, DeltaSeconds / LerpingAlphaValue);
	}
	else if (MaxWalkSpeed >= SprintThreshold && MaxWalkSpeed < SprintMaxSpeedThreshold)
	{
		// Gradually increase the speed towards the sprinting maximum speed.
		MaxWalkSpeed = FMath::Lerp(MaxWalkSpeed, SprintMaxSpeedThreshold + 50.f, DeltaSeconds / LerpingAlphaValue * ScaledLerpingAlpha);
	}
	else if (MaxWalkSpeed >= SprintMaxSpeedThreshold)
	{
		MaxWalkSpeed = Sprint_MaxWalkSpeed;
	}

	// Adjust the rotation rate for smoother turning while StartSprinting function is active.
	this->RotationRate			= FRotator(0.0f, 250.0f, 0.0f);
}

void UEnhancedMovementComponent::DecreaseMovementSpeed_AfterSprint()
{	// Stop the sprinting action if allowed. If character is crouching, exit the function.
	if (IsCrouching())
	{
		return;
	}

	if (CharacterOwner->IsLocallyControlled())
	{	
		Server_DeactiveSprint();
	}

	// If the character is currently sprinting, set the sprinting flag to false to stop sprinting.
	// Speed adjustment factors. 
	float ScalingFactor					= 5.0f * 0.25f; // A scaling factor for adjustments.
	float HalfFactor					= 2.0f;			// Half the factor for calculations.
	float NearlyStoppedThreshold		= ((Jog_MaxWalkSpeed / HalfFactor) / HalfFactor);  // Minimum velocity threshold for nearly stopped.
	float SprintToJogTransitionSpeed	= Sprint_MaxWalkSpeed - 200.f;

	// Get the time interval since the last frame.
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();

	if (MaxWalkSpeed > Sprint_MaxWalkSpeed - (Jog_MaxWalkSpeed / HalfFactor))
	{
		MaxWalkSpeed = FMath::Lerp(MaxWalkSpeed, SprintToJogTransitionSpeed, DeltaSeconds );
		float ExitSprintToJogTransitionSpeed = 650.0f;
		if (MaxWalkSpeed <= ExitSprintToJogTransitionSpeed)
		{
			MaxWalkSpeed = SprintToJogTransitionSpeed;
		}
	}
	else if (MaxWalkSpeed <= SprintToJogTransitionSpeed)
	{
		MaxWalkSpeed = FMath::Lerp(MaxWalkSpeed, Jog_MaxWalkSpeed, DeltaSeconds * 2.0f);
	}

	// Adjust the rotation rate to facilitate smoother turning.
	this->RotationRate = FRotator(0.0f, 300.f, 0.0f);

	float ExitSprintSpeedThreshold = 425.0f;
	if (CurrentVelocitySize <= ExitSprintSpeedThreshold)
	{
		MaxWalkSpeed = Jog_MaxWalkSpeed;
		bIsSprinting = false;
	}
}

bool UEnhancedMovementComponent::IsSprinting()
{
	// Check if the character is currently sprinting.
	return bIsSprinting;
}

bool UEnhancedMovementComponent::IsControllerWantToSprint() const
{
	// Check if the sprint input is active on the controller.
	return Safe_bWantsToSprint;
}

void UEnhancedMovementComponent::SetSprintInput(bool Trigger)
{
	Safe_bWantsToSprint = Trigger;
}
void UEnhancedMovementComponent::Server_ActiveSprint_Implementation()
{
	if (!bIsSprinting)
	{
		bIsSprinting = true;
	}
}
void UEnhancedMovementComponent::Server_DeactiveSprint_Implementation()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
	}
}
#pragma endregion
#pragma region | Wall Run |
bool UEnhancedMovementComponent::TryWallRun()
{
	if (GravityScale != 1.0f)
	{
		GravityScale = 1.0f;
	}

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

	FVector Start = UpdatedComponent->GetComponentLocation();
	auto IgnoredCharacterParameters = BaseCharacter->GetIgnoreCharacterParams();


	FHitResult FloorHit, WallHit;

	// Check Player Height
	FVector FloorCheckEndLocation = Start + FVector::DownVector * (CapHalfHeight() + MinWallRunHeight);

	if (GetWorld()->LineTraceSingleByProfile(FloorHit, Start, FloorCheckEndLocation, "BlockAll", IgnoredCharacterParameters)) return false;

	// Left Cast
	FVector DownDirectionOffset = FVector::DownVector * 50.0f;
	FVector WallRunCheck_StartLocation = Start + DownDirectionOffset;
	FVector LeftWallDirectionEnd = WallRunCheck_StartLocation - UpdatedComponent->GetRightVector() * CapRadius() * 2;
	FVector RightWallDirectionEnd = WallRunCheck_StartLocation + UpdatedComponent->GetRightVector() * CapRadius() * 2;


	GetWorld()->LineTraceSingleByProfile
	(
		WallHit,
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

void UEnhancedMovementComponent::PhysWallRun(float deltaTime, int32 Iterations)
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
		bJustTeleported			  = false;
		const float timeTick	  = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime			 -= timeTick;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();

		FVector Start			  = UpdatedComponent->GetComponentLocation();
		FVector CastDelta		  = UpdatedComponent->GetRightVector() * CapRadius() * 2;
		FVector End				  = Safe_bWallRunIsRight ? Start + CastDelta : Start - CastDelta;

		auto IgnoredCharacterParameters = BaseCharacter->GetIgnoreCharacterParams();
		float SinPullAwayAngle			= FMath::Sin(FMath::DegreesToRadians(WallRunPullAwayAngle));

		FHitResult WallHit;
		GetWorld()->LineTraceSingleByProfile(WallHit, Start, End, "BlockAll", IgnoredCharacterParameters);

		bool bWantsToPullAway =
			WallHit.IsValidBlockingHit() &&
			!Acceleration.IsNearlyZero() &&
			(Acceleration.GetSafeNormal() | WallHit.Normal) > SinPullAwayAngle;

		if (!WallHit.IsValidBlockingHit() || bWantsToPullAway)
		{
			SetMovementMode(MOVE_Falling);
			StartNewPhysics(remainingTime, Iterations);
			return;
		}

		// Clamp Acceleration
		Acceleration   = FVector::VectorPlaneProject(Acceleration, WallHit.Normal);
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

		//Velocity.Z += GetGravityZ() * WallRunGravityScaleCurve->GetFloatValue(bVelocityUp ? 0.f : TangentAccel) * timeTick;
	
		GravityResetTimer += GetWorld()->GetDeltaSeconds();

		if (GravityResetTimer <= GravityThresholdDelay)
		{
			Velocity.Z -= GetGravityZ() * WallRunGravityScaleCurve->GetFloatValue(0.014f);
		}
		else
		{
			Velocity.Z += GetGravityZ() * WallRunGravityScaleCurve->GetFloatValue(0.014f);
		}

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



	FVector WallExitStart = Start + UpdatedComponent->GetForwardVector() * 50.0f;
	FVector WallExitCastDelta = UpdatedComponent->GetRightVector() * CapRadius() * 2;

	FVector WallExitEndRight = WallExitStart + WallExitCastDelta;
	FVector WallExitEndLeft = WallExitStart - WallExitCastDelta;

	FHitResult WallExitHitRight;
	FHitResult WallExitHitLeft;


	if (Safe_bWallRunIsRight)
	{
		if (!GetWorld()->LineTraceSingleByProfile(WallExitHitRight, WallExitStart, WallExitEndRight, "BlockAll", IgnoredCharacterParameters))
		{
			CharacterOwner->Jump();
		}
	}
	else
	{
		if (!GetWorld()->LineTraceSingleByProfile(WallExitHitLeft, WallExitStart, WallExitEndLeft, "BlockAll", IgnoredCharacterParameters))
		{
			CharacterOwner->Jump();
		}
	}

	MaxWallRunDelay += GetWorld()->GetDeltaSeconds();

	if (MaxWallRunDelay >= MaxWallRunTime)
	{
		SetMovementMode(MOVE_Falling);
		MaxWallRunDelay = 0.0f;
	}

	if (FloorHit.IsValidBlockingHit() || !WallHit.IsValidBlockingHit() || Velocity.SizeSquared2D() < pow(WallRun_MinWalkSpeed, 2))
	{
		SetMovementMode(MOVE_Falling);
	}
}
#pragma endregion
#pragma region | Climb And Hang |

void UEnhancedMovementComponent::OnRep_ShortMantle()
{
	BaseCharacter->Head->GetAnimInstance()->Montage_Play(ProxyShortMantleMontage);
}

void UEnhancedMovementComponent::OnRep_TallMantle()
{
	BaseCharacter->Head->GetAnimInstance()->Montage_Play(ProxyTallMantleMontage);
}


//bool UEnhancedMovementComponent::TryClimb()
//{
//	if (!IsFalling())
//	{
//		return false;
//	}
//
//	FHitResult SurfaceHit;
//	FVector CapsuleLocation = UpdatedComponent->GetComponentLocation();
//
//	GetWorld()->LineTraceSingleByProfile(SurfaceHit, CapsuleLocation, CapsuleLocation + UpdatedComponent->GetForwardVector() * Climb_ReachDistance, "BlockAll", BaseCharacter->GetIgnoreCharacterParams());
//
//	if (!SurfaceHit.IsValidBlockingHit()) return false;
//
//	FQuat NewRotation = FRotationMatrix::MakeFromXZ(-SurfaceHit.Normal, FVector::UpVector).ToQuat();
//	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, false, SurfaceHit);
//
//	SetMovementMode(MOVE_Custom, CMOVE_Climb);
//
//	bOrientRotationToMovement = false;
//
//	return true;
//}
//void UEnhancedMovementComponent::PhysClimb(float deltaTime, int32 Iterations)
//{
//	if (deltaTime < MIN_TICK_TIME)
//	{
//		return;
//	}
//
//	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
//	{
//		Acceleration = FVector::ZeroVector;
//		Velocity = FVector::ZeroVector;
//		return;
//	}
//
//	// Perform the move
//	bJustTeleported = false;
//	Iterations++;
//	const FVector OldLocation = UpdatedComponent->GetComponentLocation();
//	FHitResult SurfaceHit, FloorHit;
//	GetWorld()->LineTraceSingleByProfile(SurfaceHit, OldLocation, OldLocation + UpdatedComponent->GetForwardVector() * Climb_ReachDistance, "BlockAll", BaseCharacter->GetIgnoreCharacterParams());
//	GetWorld()->LineTraceSingleByProfile(FloorHit, OldLocation, OldLocation + FVector::DownVector * CapHalfHeight() * 1.2f, "BlockAll", BaseCharacter->GetIgnoreCharacterParams());
//	if (!SurfaceHit.IsValidBlockingHit() || FloorHit.IsValidBlockingHit())
//	{
//		SetMovementMode(MOVE_Falling);
//		StartNewPhysics(deltaTime, Iterations);
//		return;
//	}
//
//	// Transform Acceleration
//	Acceleration.Z = 0.f;
//	Acceleration = Acceleration.RotateAngleAxis(90.f, -UpdatedComponent->GetRightVector());
//
//	// Apply acceleration
//	CalcVelocity(deltaTime, 0.f, false, GetMaxBrakingDeceleration());
//	Velocity = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal);
//
//	// Compute move parameters
//	const FVector Delta = deltaTime * Velocity; // dx = v * dt
//	if (!Delta.IsNearlyZero())
//	{
//		FHitResult Hit;
//		SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);
//		FVector WallAttractionDelta = -SurfaceHit.Normal * WallAttractionForce * deltaTime;
//		SafeMoveUpdatedComponent(WallAttractionDelta, UpdatedComponent->GetComponentQuat(), true, Hit);
//	}
//
//	Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime; // v = dx / dt
//}


bool UEnhancedMovementComponent::TryMantle()
{
	if (!(IsMovementMode(MOVE_Walking) && !IsCrouching()) && !IsMovementMode(MOVE_Falling)) return false;


	FVector BaseLocation = UpdatedComponent->GetComponentLocation() + FVector::DownVector * CapHalfHeight();
	FVector Forward		 = UpdatedComponent->GetForwardVector().GetSafeNormal2D();
	auto IgnoredParams	 = BaseCharacter->GetIgnoreCharacterParams();
	float MaxHeight		 = CapHalfHeight() * 2 + MantleReachHeight;
	float CosMMWSA		 = FMath::Cos(FMath::DegreesToRadians(MantleMinWallSteepnessAngle));
	float CosMMSA		 = FMath::Cos(FMath::DegreesToRadians(MantleMaxSurfaceAngle));
	float CosMMAA		 = FMath::Cos(FMath::DegreesToRadians(MantleMaxAlignmentAngle));

	// Check Front Face
	FHitResult FrontHit;
	float CheckDistance = FMath::Clamp(Velocity | Forward, CapRadius() + 30, MantleMaxDistance);
	FVector FrontStart = BaseLocation + FVector::UpVector * (MaxStepHeight - 1);

	for (int i = 0; i < 6; i++)
	{

		if (GetWorld()->LineTraceSingleByProfile(FrontHit, FrontStart, FrontStart + Forward * CheckDistance, "BlockAll", IgnoredParams))
		{
			break;
		}
		FrontStart += FVector::UpVector * (2.f * CapHalfHeight() - (MaxStepHeight - 1)) / 5;
	}

	if (!FrontHit.IsValidBlockingHit())
	{
		return false;
	}

	float CosineWallSteepnessAngle = FrontHit.Normal | FVector::UpVector;
	if (FMath::Abs(CosineWallSteepnessAngle) > CosMMWSA || (Forward | -FrontHit.Normal) < CosMMAA)
	{
		return false;
	}

	// Height Check
	TArray<FHitResult> HeightHits;
	FHitResult SurfaceHit;
	FVector WallUp     = FVector::VectorPlaneProject(FVector::UpVector, FrontHit.Normal).GetSafeNormal();

	float WallCosine   = FVector::UpVector | FrontHit.Normal;
	float WallSine     = FMath::Sqrt(1 - WallCosine * WallCosine);
	
	FVector TraceStart = FrontHit.Location + Forward + WallUp * (MaxHeight - (MaxStepHeight - 1)) / WallSine;


	if (!GetWorld()->LineTraceMultiByProfile(HeightHits, TraceStart, FrontHit.Location + Forward, "BlockAll", IgnoredParams))
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
	float SurfaceCosine = FVector::UpVector | SurfaceHit.Normal;
	float SurfaceSine   = FMath::Sqrt(1 - SurfaceCosine * SurfaceCosine);

	FVector ClearCapsuleLocation = SurfaceHit.Location + Forward * CapRadius() + FVector::UpVector * (CapHalfHeight() + 1 + CapRadius() * 2 * SurfaceSine);
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapRadius(), CapHalfHeight());



	if (GetWorld()->OverlapAnyTestByProfile(ClearCapsuleLocation, FQuat::Identity, "BlockAll", CapsuleShape, IgnoredParams))
	{
		return false;
	}


	//Mantle type selection
	ShortMantleTarget = GetMantleStartLocation(FrontHit, SurfaceHit, false);
	TallMantleTarget  = GetMantleStartLocation(FrontHit, SurfaceHit, true);

	bTallMantle = false;

	if (Height > CapHalfHeight() * 2)
	{
		bTallMantle = true;
	}
	else if (IsMovementMode(MOVE_Falling) && (Velocity | FVector::UpVector) < 0)
	{
		if (!GetWorld()->OverlapAnyTestByProfile(TallMantleTarget, FQuat::Identity, "BlockAll", CapsuleShape, IgnoredParams))
		{
			bTallMantle = true;
		}
	}

	Mantle_TargetLocation = ClearCapsuleLocation;
	Mantle_UpTransitionTarget = UpdatedComponent->GetComponentLocation() + FVector::UpVector * CapHalfHeight() * 4.f;
	return true;
}

void UEnhancedMovementComponent::SetMantleLocation()
{
	if (!bCanMantle)  return;

	UCameraSettings* CameraSettingsInstance = Cast<UCameraSettings>(GetOwner()->GetComponentByClass(UCameraSettings::StaticClass()));

	if (CameraSettingsInstance)
	{
		CameraSettingsInstance->Client_ClimbCameraShake();
	}

	if (bTallMantle)
	{	
		if (BaseCharacter->HasAuthority())
		{
			Multicast_SetTallMantleLocation();
			Proxy_bTallMantle = !Proxy_bTallMantle;
		}
	}
	else
	{
		if (MantleTransitionTime <= MantleTransitionDuration)
		{
			if (!bIsMantle)
			{
				if (BaseCharacter->HasAuthority())
				{
					Proxy_bShortMantle = !Proxy_bShortMantle;
				}

				if (bIsMantle != true)
				{
					bIsMantle = true;
				}
			}
			
			if (bIsMantle)
			{
				MantleTransitionTime += GetWorld()->GetDeltaSeconds();

				float Alpha = FMath::Clamp(MantleTransitionTime / MantleTransitionDuration, 0.0f, 6.0f * GetWorld()->GetDeltaSeconds());
				FVector TargetLocation = FMath::Lerp(UpdatedComponent->GetComponentLocation(), Mantle_TargetLocation, Alpha);
				BaseCharacter->SetActorLocationAndRotation(TargetLocation, BaseCharacter->GetActorRotation());
				if (Velocity.Size() > 100.f)
				{
					Velocity.Z = 0.f;
					if (MantleTransitionTime >= 0.7f)
					{

						if (bIsMantle)
						{
							bIsMantle = false;
						}
		
						MantleTransitionTime = 0.0f;
					}
				}
				else
				{
					if (MantleTransitionTime >= 0.45f)
					{
						if (bIsMantle)
						{
							bIsMantle = false;
						}
						if (bCanMantle)
						{
							bCanMantle = false;
						}
						MantleTransitionTime = 0.0f;
					}
				}
			}
		}
	}
}

void UEnhancedMovementComponent::Multicast_SetTallMantleLocation_Implementation()
{
	if (!bIsMantle)
	{
		bIsMantle = true;
	}
	MantleTransitionTime += GetWorld()->GetDeltaSeconds();

	if (MantleTransitionTime < .4f)
	{
		if (bMantleMovementModeGate)
		{
			if (GravityScale != 0.0f)
				GravityScale = 0.f;
			StopMovementImmediately();
			bMantleMovementModeGate = false;
		}
		float Alpha = FMath::Clamp(MantleTransitionTime / MantleTransitionDuration, 0.0f, 3.0f * GetWorld()->GetDeltaSeconds());
		FVector TargetLocation = FMath::Lerp(UpdatedComponent->GetComponentLocation(), Mantle_UpTransitionTarget, Alpha);
		BaseCharacter->SetActorLocationAndRotation(TargetLocation, BaseCharacter->GetActorRotation(), false, nullptr, ETeleportType::ResetPhysics);
	}
	else if (MantleTransitionTime >= .4f && MantleTransitionTime < .8f)
	{
		float Alpha = FMath::Clamp(MantleTransitionTime / MantleTransitionDuration, 0.0f, 9.0f * GetWorld()->GetDeltaSeconds());
		FVector TargetLocation = FMath::Lerp(UpdatedComponent->GetComponentLocation(), Mantle_TargetLocation, Alpha);
		BaseCharacter->SetActorLocationAndRotation(TargetLocation, BaseCharacter->GetActorRotation(), false, nullptr, ETeleportType::ResetPhysics);
	}
	else if (MantleTransitionTime >= .8f)
	{
		SetMovementMode(MOVE_Walking);
		GravityScale = 1.0f;
		MantleTransitionTime = 0.0f;
		if (bIsMantle)
			bIsMantle = false;
		if (bCanMantle)
			bCanMantle = false;
		bMantleMovementModeGate = true;
	}
}

FVector UEnhancedMovementComponent::GetMantleStartLocation(FHitResult FrontHit, FHitResult SurfaceHit, bool bIsTallMantle) const
{
	float CosineWallSteepnessAngle = FrontHit.Normal | FVector::UpVector;
	float DownDistance = bIsTallMantle ? CapHalfHeight() * 2.0f : MaxStepHeight - 1;

	FVector EdgeTangent = FVector::CrossProduct(SurfaceHit.Normal, FrontHit.Normal).GetSafeNormal();

	FVector MantleStart = SurfaceHit.Location;
	MantleStart += FrontHit.Normal.GetSafeNormal2D() * (2.0f + CapRadius());
	MantleStart += UpdatedComponent->GetForwardVector().GetSafeNormal2D().ProjectOnTo(EdgeTangent) * CapRadius() * .3f;
	MantleStart += FVector::UpVector * CapHalfHeight();
	MantleStart += FVector::DownVector * DownDistance;
	MantleStart += FrontHit.Normal.GetSafeNormal2D() * CosineWallSteepnessAngle * DownDistance;

	return MantleStart;
}

#pragma endregion
#pragma region  | Crouch - System |
bool UEnhancedMovementComponent::CanCrouch()
{
	return 
		!CharacterOwner->bIsCrouched								&&
		 CanEverCrouch()											&&
		 CharacterOwner->GetRootComponent()							&& 
		!CharacterOwner->GetRootComponent()->IsSimulatingPhysics()	&&
		!IsSprinting() && IsMovingOnGround();
}
bool UEnhancedMovementComponent::IsCrouching_Custom()
{
	return bIsCrouched;
}
#pragma endregion
#pragma region | Slide - System |
bool UEnhancedMovementComponent::CanSlide()
{
	FVector Start		= UpdatedComponent->GetComponentLocation();
	FVector End			= Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.5f * FVector::DownVector;
	FName ProfileName	= TEXT("BlockAll");
	bool bValidSurface	= GetWorld()->LineTraceTestByProfile(Start, End, ProfileName, BaseCharacter->GetIgnoreCharacterParams());
	bool bEnoughSpeed	= Velocity.SizeSquared() > pow(Slide_MinSpeed, 2);

	return 
		bValidSurface &&
		bEnoughSpeed;
}

bool UEnhancedMovementComponent::IsSliding()
{
	return bIsSliding;
}

void UEnhancedMovementComponent::EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode)
{
	if (CharacterOwner->HasAuthority())
	{
		Server_EnterSlide(PrevMode, PrevCustomMode);
	}
}


void UEnhancedMovementComponent::Server_EnterSlide_Implementation(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode)
{
	if (bIsSliding != true)
	{
		bIsSliding = true;
		bOrientRotationToMovement = false;
		Velocity += Velocity.GetSafeNormal2D() * Slide_EnterImpulse;
		FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);
	}
}


void UEnhancedMovementComponent::Multicast_EnterSlide_Implementation(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode)
{

}

void UEnhancedMovementComponent::ExitSlide()
{
	if (CharacterOwner->IsLocallyControlled())
	{
		if (BaseCharacter->bPressedJump)
		{
			float JumpValue = JumpZVelocity * CurrentVelocitySize / 50.0f;
			FVector JumpVector = FVector(0.f, 0.f, JumpValue);
			BaseCharacter->LaunchCharacter(JumpVector, false, false);
		}
		else
		{
			bIsSliding = false;
			bWantsToSlide = false;
			bOrientRotationToMovement = false;
			SetMovementMode(MOVE_Walking);
		}
	}
}

void UEnhancedMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CanSlide())
	{
		SetMovementMode(MOVE_Walking);
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	bJustTeleported = false;
	bool bCheckedFall = false;
	bool bTriedLedgeMove = false;
	float remainingTime = deltaTime;

	// Perform the move 
	while ((remainingTime >= MIN_TICK_TIME) &&
		(Iterations < MaxSimulationIterations) &&
		CharacterOwner	&&
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
			const FVector GravDir = FVector(0.f, 0.f, -1.f);
			const FVector NewDelta = bTriedLedgeMove ? FVector::ZeroVector : GetLedgeMove(OldLocation, Delta, GravDir);
			if (!NewDelta.IsZero())
			{
				// first revert this move
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
				bool bMustJump = bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
				{

					return;
				}
				bCheckedFall = true;

				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
				remainingTime = 0.f;
				break;
			}
		}
		else
		{
			// Validate the floor check
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

		if (IsMovingOnGround() && bFloorWalkable)
		{
			if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && timeTick >= MIN_TICK_TIME)
			{
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

void UEnhancedMovementComponent::SetSlideInput(bool Trigger)
{
	bWantsToSlide = Trigger;
}

bool UEnhancedMovementComponent::IsControllerWantsSlide()
{
	return bWantsToSlide;
}
#pragma endregion
#pragma region | Prone - System |

bool UEnhancedMovementComponent::CanProne()
{
	return	IsCustomMovementMode(CMOVE_Slide) ||
		IsMovementMode(MOVE_Walking) && IsCrouching() ||
		IsMovementMode(MOVE_Walking) ||
		!IsSprinting();
}

bool UEnhancedMovementComponent::IsProning()
{
	return bIsProning;
}

bool UEnhancedMovementComponent::IsControllerWantsToProne() const
{
	return Safe_bWantsToProne;
}

void UEnhancedMovementComponent::SetProneInput(bool Trigger)
{
	Safe_bWantsToProne = Trigger;
}

void UEnhancedMovementComponent::Server_EnterProne_Implementation()
{
	Safe_bWantsToProne = true;
}

void UEnhancedMovementComponent::Server_ExitProne_Implementation()
{
	Safe_bWantsToProne = false;
}

void UEnhancedMovementComponent::Multicast_EnterProne_Implementation()
{
	Safe_bWantsToProne = true;
}

void UEnhancedMovementComponent::Multicast_ExitProne_Implementation()
{
	Safe_bWantsToProne = false;
}

void UEnhancedMovementComponent::EnterProne(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode)
{

	if (PrevMode == MOVE_Custom && PrevCustomMode == CMOVE_Slide)
	{
		Velocity += Velocity.GetSafeNormal2D() * Prone_SlideEnterImpulse;
	}

	if (IsCrouching())
	{
		bWantsToCrouch = false;
	}

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);
}

void UEnhancedMovementComponent::ExitProne()
{
	SetMovementMode(MOVE_Walking);
	if (bIsProning != false)
		bIsProning = false;
}

void UEnhancedMovementComponent::PhysProne(float deltaTime, int32 Iterations)
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

	bJustTeleported			= false;
	bool bCheckedFall		= false;
	bool bTriedLedgeMove	= false;
	float remainingTime		= deltaTime;
	
	while ((remainingTime >= MIN_TICK_TIME)			&& 
			(Iterations < MaxSimulationIterations)	&&
			CharacterOwner							&& 
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

		// Ensure velocity is horizontal 
		MaintainHorizontalGroundVelocity();
		const FVector OldVelocity = Velocity;
		Acceleration.Z = 0.0f;

		//Apply Acceleration 
		CalcVelocity(timeTick, GroundFriction, false, GetMaxBrakingDeceleration());

		// Compute move parameters
		const FVector MoveVelocity = Velocity;
		const FVector Delta = timeTick * MoveVelocity; // dx = v * dt;
		const bool bZeroDelta = Delta.IsNearlyZero();
		FStepDownResult StepDownResult;

		if (bZeroDelta)
		{
			remainingTime = 0.f;
		}
		else
		{
			MoveAlongFloor(MoveVelocity, timeTick, &StepDownResult);

			if (IsFalling())
			{
				// Pawn decided to jump up
				const float DesiredDistance = Delta.Size();
				if (DesiredDistance > KINDA_SMALL_NUMBER)
				{
					const float ActualDistance = (UpdatedComponent->GetComponentLocation() - OldLocation).Size2D();
					remainingTime += timeTick * (1.f - FMath::Min(1.f, ActualDistance / DesiredDistance));
				}
				StartNewPhysics(remainingTime, Iterations);
				return;
			}
			else if (IsSwimming()) // just entered water
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
			// Calculate possible alternate movement
			const FVector Local_GravityDirection = FVector(0.0f, 0.0f, -1.0f);
			const FVector NewDelta = bTriedLedgeMove ? FVector::ZeroVector : GetLedgeMove(OldLocation, Delta, Local_GravityDirection);

			if (!NewDelta.IsZero())
			{
				// First revert this move 
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, false);

				// Avoid repeated ledge moves if the first one fails
				bTriedLedgeMove = true;

				// Try new movement direction
				Velocity = NewDelta / timeTick;
				remainingTime += timeTick;
				continue;
			}
			else
			{
				bool bMustJump = bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
				{
					return;
				}
				bCheckedFall = true;

				// revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
				remainingTime = 0.0f;
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
			else if (CurrentFloor.HitResult.bStartPenetrating && remainingTime <= 0.0f)
			{
				// the floor check failed because it started in penetration
				// We do not want to try to move downward because the donwrad sweep failed, rather we'd like to try to pop out of the floor.
				FHitResult Hit(CurrentFloor.HitResult);
				Hit.TraceEnd = Hit.TraceStart + FVector(0.0f, 0.0f, MAX_FLOOR_DIST);
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

			// See if we need to start failing.
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
	if (IsMovingOnGround())
	{
		MaintainHorizontalGroundVelocity();
	}
}
#pragma endregion 
#pragma region | Hanging |
//bool UEnhancedMovementComponent::TryHang()
//{
//	if (!IsMovementMode(MOVE_Falling))
//	{
//		return false;
//	}
//
//
//	FHitResult WallHit;
//	if (!GetWorld()->LineTraceSingleByProfile(WallHit, UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + UpdatedComponent->GetForwardVector() * 300.0f, "BlockAll", BaseCharacter->GetIgnoreCharacterParams()))
//	{
//		return false;
//	}
//
//	TArray<FOverlapResult> OverlapResults;
//
//	FVector CollisionLocation = UpdatedComponent->GetComponentLocation() + FVector::UpVector * CapHalfHeight() + UpdatedComponent->GetForwardVector() * CapRadius() * 3;
//	auto CollisionBox		  = FCollisionShape::MakeBox(FVector(100.0f, 100.0f, 50.0f));
//	FQuat CollisionRot		  = FRotationMatrix::MakeFromXZ(WallHit.Normal, FVector::UpVector).ToQuat();
//
//	if (!GetWorld()->OverlapMultiByChannel(OverlapResults, CollisionLocation, CollisionRot, ECC_WorldStatic, CollisionBox, BaseCharacter->GetIgnoreCharacterParams()))
//	{
//		return false;
//	}
//
//	AActor* ClimbPoint = nullptr;
//	float MaxHeight = -1e20;
//
//	for (FOverlapResult Result : OverlapResults)
//	{
//		if (Result.GetActor()->ActorHasTag("Climb Point"))
//		{
//			float Height = Result.GetActor()->GetActorLocation().Z;
//
//			if (Height > MaxHeight)
//			{
//				MaxHeight = Height;
//				ClimbPoint = Result.GetActor();
//			}
//		}
//	}
//
//	if (!IsValid(ClimbPoint))
//	{
//		return false;
//	}
//
//
//
//	FVector TargetLocation = ClimbPoint->GetActorLocation() + WallHit.Normal * CapRadius() * 1.01f + FVector::DownVector * CapHalfHeight();
//	FQuat TargetRotation = FRotationMatrix::MakeFromXZ(-WallHit.Normal, FVector::UpVector).ToQuat();
//
//	FTransform CurrentTransform = UpdatedComponent->GetComponentTransform();
//	FHitResult Hit, ReturnHit;
//	SafeMoveUpdatedComponent(TargetLocation - UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentQuat(), true, Hit);
//	FVector ResultLocation = UpdatedComponent->GetComponentLocation();
//	SafeMoveUpdatedComponent(CurrentTransform.GetLocation() - ResultLocation, TargetRotation, false, ReturnHit);
//
//	//if (!ResultLocation.Equals(TargetLocation))
//	//{
//	//	return false;
//	//}
//
//	// Passed all conditions
//	bOrientRotationToMovement = false;
//
//	// Perform Transition to CLimb point
//	float UpSpeed = Velocity | FVector::UpVector;
//	float TransitionDistance = FVector::Dist(TargetLocation, UpdatedComponent->GetComponentLocation());
//
//	TransitionQueuedMontageSpeed = FMath::GetMappedRangeValueClamped(FVector2D(-500.0f, 750.0f), FVector2D(0.9f, 1.2f), UpSpeed);
//	TransitionRMS.Reset();
//	TransitionRMS = MakeShared<FRootMotionSource_MoveToForce>();
//	TransitionRMS->AccumulateMode = ERootMotionAccumulateMode::Override;
//
//	TransitionRMS->Duration = FMath::Clamp(TransitionDistance / 500.f, 0.1f, 0.25f);
//	TransitionRMS->StartLocation = UpdatedComponent->GetComponentLocation();
//	TransitionRMS->TargetLocation = TargetLocation;
//
//	// Apply Transition Root Motion Source
//	Velocity = FVector::ZeroVector;
//	SetMovementMode(MOVE_Flying);
//	TransitionRMS_ID = ApplyRootMotionSource(TransitionRMS);
//
//	// Animations
//	TransitionQueuedMontage = nullptr;
//	TransitionName = "Hang";
//	BaseCharacter->PlayAnimMontage(TransitionHangMontage, 1 / TransitionRMS->Duration);
//	LOG_WARNING("PLAY ANIM")
//	return true;
//}
#pragma endregion




void UEnhancedMovementComponent::DebugEachClient()
{

}


void UEnhancedMovementComponent::CalculateSlopeAngle(float DeltaTime)
{
	FVector ActorLocation = GetActorLocation();
	FVector StartLocation = ActorLocation + (BaseCharacter->GetActorForwardVector() * 35.f);
	FVector End			  = StartLocation + (BaseCharacter->GetActorForwardVector() + FVector(0.f, 0.f, -500.f));

	FCollisionQueryParams TraceParamaters;
	TraceParamaters.bTraceComplex = false;
	ECollisionChannel TraceChannelVisiblity = ECollisionChannel::ECC_Visibility;
	TraceParamaters.AddIgnoredActor(BaseCharacter);

	FHitResult HitResult;
	UWorld* World	= GetWorld();
	bool bHit		= GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, End, TraceChannelVisiblity, TraceParamaters);

	bool bBlockingHit, bInitialOverlap;
	float Time, Distance;
	FVector Location, ImpactPoint, Normal, ImpactNormal, TraceStart, TraceEnd;
	class UPhysicalMaterial* PhysicMaterial;
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
		PhysicMaterial,
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

	FVector Right = BaseCharacter->GetActorRightVector();
	FVector Up = BaseCharacter->GetActorUpVector();
	UKismetMathLibrary::GetSlopeDegreeAngles(Right, ImpactNormal, Up, OutSlopePitchDegreeAngle, OutSlopeRollDegreeAngle);

	if (HitResult.bBlockingHit &&
		((OutSlopePitchDegreeAngle > 25.f && OutSlopePitchDegreeAngle < 39.f) ||
		 (OutSlopePitchDegreeAngle < -25.f && OutSlopePitchDegreeAngle > -39.f)) &&
		 !bSlopeHitInRange)
	{
		if (!InSlopeMovement)
		{
			InSlopeMovement = true;
		}
		if (!bVeryHighSlope)
		{
			bVeryHighSlope = false;
		}
		if (!bSlopeHitInRange)
		{
			bSlopeHitInRange = true;
		}
	}
	else if (HitResult.bBlockingHit && (OutSlopePitchDegreeAngle > 39.f || OutSlopePitchDegreeAngle < 39.f) && !bSlopeHitInRange)
	{
		if (!InSlopeMovement)
		{
			InSlopeMovement = true;
		}
		if (!bVeryHighSlope)
		{
			bVeryHighSlope = true;
		}
		if (bSlopeHitInRange)
		{
			bSlopeHitInRange = true;
		}
	}
	else if (HitResult.bBlockingHit && OutSlopePitchDegreeAngle == 0 && bSlopeHitInRange)
	{
		SlopeDegreeResetTimer += DeltaTime;

		if (SlopeDegreeResetTimer >= .5f)
		{
			SlopeDegreeResetTimer = 0.f;
			if (bVeryHighSlope)
			{
				bVeryHighSlope = false;
			}
			if (InSlopeMovement)
			{
				InSlopeMovement = false;
			}
		}
		if (!InSlopeMovement)
		{
			if (bSlopeHitInRange)
			{
				bSlopeHitInRange = false;
			}
			SlopeDegreeResetTimer = 0.f;
		}
	}
}


float UEnhancedMovementComponent::GetSlopePitchDegreeAngle() const
{
	// Returns the calculated slope pitch degree angle from the last update of the character's pitch slope calculation.
	return OutSlopePitchDegreeAngle;
}
float UEnhancedMovementComponent::GetSlopRollDegreeAngle() const
{
	// Returns the calculated slope roll degree angle from the last update of the character's pitch slope calculation.
	return OutSlopeRollDegreeAngle;
}