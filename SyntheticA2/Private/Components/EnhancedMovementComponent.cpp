// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|


#include "Components/EnhancedMovementComponent.h"

#include "Characters/BaseCharacter.h"

#include "Net/UnrealNetwork.h"


#pragma region Constructor
UEnhancedMovementComponent::UEnhancedMovementComponent()
{
	SetIsReplicated(true);
	MaxWalkSpeed					= 400.0f;


	SetCrouchedHalfHeight(60.0f);
	MaxWalkSpeedCrouched			= 175.0f;
	bCanWalkOffLedgesWhenCrouching	= true;
	NavAgentProps.bCanCrouch		= true;

}
#pragma endregion
#pragma region SavedMove
UEnhancedMovementComponent::FSavedMove_Custom::FSavedMove_Custom()
{
	Saved_bWantsToSprint = 0;
}

bool UEnhancedMovementComponent::FSavedMove_Custom::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_Custom* NewCustomMove = static_cast<FSavedMove_Custom*>(NewMove.Get());

	if (Saved_bWantsToSprint != NewCustomMove->Saved_bWantsToSprint)
	{
		return false;
	}

	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UEnhancedMovementComponent::FSavedMove_Custom::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}

uint8 UEnhancedMovementComponent::FSavedMove_Custom::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if (Saved_bWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

void UEnhancedMovementComponent::FSavedMove_Custom::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	const UEnhancedMovementComponent* EnhancedMovementComponent = Cast<UEnhancedMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = EnhancedMovementComponent->Safe_bWantsToSprint;
}

void UEnhancedMovementComponent::FSavedMove_Custom::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UEnhancedMovementComponent* EnhancedMovementComponent = Cast<UEnhancedMovementComponent>(C->GetCharacterMovement());

	EnhancedMovementComponent->Safe_bWantsToSprint = Saved_bWantsToSprint;

}
#pragma endregion
#pragma region Network
UEnhancedMovementComponent::FNetworkPredictionData_Client_Custom::FNetworkPredictionData_Client_Custom(const UCharacterMovementComponent& ClientMovement)
: Super(ClientMovement)
{

}

FSavedMovePtr UEnhancedMovementComponent::FNetworkPredictionData_Client_Custom::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Custom());
}

FNetworkPredictionData_Client* UEnhancedMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

	if (ClientPredictionData == nullptr)
	{
		UEnhancedMovementComponent* MutableThis = const_cast<UEnhancedMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Custom(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.0f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.0f;
	}

	return ClientPredictionData;
}

void UEnhancedMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{

	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}
#pragma endregion
#pragma region EnhancedCustomMovementComponent
void UEnhancedMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	BaseCharacter = Cast<ABaseCharacter>(GetOwner());
}

void UEnhancedMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnhancedMovementComponent, bIsSprinting);
}

void UEnhancedMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);
}

void UEnhancedMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UEnhancedMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (IsControllerWantToSprint())
		{
			SprintButtonPressed();
		}
		else
		{
			SprintButtonReleased();
		}
	}

}
#pragma endregion
#pragma region Sprint
bool UEnhancedMovementComponent::CanSprint()
{
	return 
		Super::IsMovingOnGround() &&
		!IsCrouching() &&
		!IsFalling();
}

void UEnhancedMovementComponent::SetSprintInput(bool SetValue)
{
	Safe_bWantsToSprint = SetValue;
}

bool UEnhancedMovementComponent::IsSprinting()
{
	return bIsSprinting;
}

bool UEnhancedMovementComponent::IsControllerWantToSprint() const
{
	return Safe_bWantsToSprint;
}

void UEnhancedMovementComponent::SprintButtonPressed()
{
	if (!IsControllerWantToSprint())
	{
		bIsSprinting = false;
		MaxWalkSpeed = Jog_MaxWalkSpeed;
		SetSprintInput(false);
		return;
	}

	bIsSprinting = true;
	if (bSwitchSprint)
	{
		MaxWalkSpeed = Sprint_MaxThresholdSpeed;

	}
	else
	{
		MaxWalkSpeed = Sprint_MaxWalkSpeed;
	}

	CurrentVelocitySize = BaseCharacter->GetVelocity().Size();
	if (CurrentVelocitySize < Jog_MaxWalkSpeed)
	{
		if (MaxWalkSpeed != Jog_MaxWalkSpeed)
		{
			SetSprintInput(false);
			MaxWalkSpeed = Jog_MaxWalkSpeed;
			bSwitchSprint = false;
		}
	}
}

void UEnhancedMovementComponent::SprintButtonReleased()
{
	if (IsControllerWantToSprint()) return;

	if (MaxWalkSpeed <= Jog_MaxWalkSpeed) return;

	CurrentVelocitySize = BaseCharacter->GetVelocity().Size();
	if (CurrentVelocitySize > Jog_MaxWalkSpeed)
	{
		DecreaseSprintSpeed();
	}
	else
	{
		if (MaxWalkSpeed != Jog_MaxWalkSpeed)
		{
			MaxWalkSpeed = Jog_MaxWalkSpeed;
		}
	}
}

void UEnhancedMovementComponent::IncreaseSprintSpeed()
{
	if (CurrentVelocitySize < Jog_MaxWalkSpeed && !IsControllerWantToSprint())
	{
		MaxWalkSpeed = Jog_MaxWalkSpeed;
		SetSprintInput(false);
	}

	//if (MaxWalkSpeed >= Sprint_MaxWalkSpeed)
	//{
	//	if(MaxWalkSpeed != Sprint_MaxWalkSpeed)
	//		MaxWalkSpeed = Sprint_MaxWalkSpeed;

	//	if (!bIsSprinting != true) 
	//		bIsSprinting = true;
	//	return;
	//}
	//// Speed adjustment factors.
	//float ScalingFactor				= 5.0f * 0.25f; // A scaling factor for adjustments.
	//float HalfFactor				= 2.0f;			// Half the factor for calculations.
	//float NearlyStoppedThreshold	= ((Jog_MaxWalkSpeed / HalfFactor) / HalfFactor); // Minimum velocity threshold for nearly stopped.

	//// Get the time interval since the last frame.
	//float DeltaSeconds				= GetWorld()->GetDeltaSeconds();

	//// Calculate the threshold speed for transitioning to sprinting.
	//float SprintThreshold			= ((Jog_MaxWalkSpeed + Sprint_MaxWalkSpeed) / HalfFactor);
	//float SprintMaxSpeedThreshold	= 750.0f;

	//// Factor for controlling the speed transition smoothness.
	//float LerpingAlphaValue			= 2.0f;						 // Controls how quickly the speed changes.
	//float ScaledLerpingAlpha		= LerpingAlphaValue * 0.75f; // Scaled value for more gradual speed changes.

	//// Adjust the character's speed based on the current state.
	//if (MaxWalkSpeed >= Jog_MaxWalkSpeed && MaxWalkSpeed < SprintThreshold)
	//{
	//	bIsSprinting = true;
	//	// Gradually increase the speed towards the sprint threshold.
	//	MaxWalkSpeed = FMath::Lerp(MaxWalkSpeed, SprintThreshold + 100.0f, DeltaSeconds / LerpingAlphaValue);
	//}
	//else if (MaxWalkSpeed >= SprintThreshold && MaxWalkSpeed < SprintMaxSpeedThreshold)
	//{
	//	// Gradually increase the speed towards the sprinting maximum speed.
	//	MaxWalkSpeed = FMath::Lerp(MaxWalkSpeed, SprintMaxSpeedThreshold + 50.f, DeltaSeconds / LerpingAlphaValue * ScaledLerpingAlpha);
	//}
	//else if (MaxWalkSpeed >= SprintMaxSpeedThreshold)
	//{
	//	MaxWalkSpeed = Sprint_MaxWalkSpeed;
	//}

	// Adjust the rotation rate for smoother turning while StartSprinting function is active.
	this->RotationRate = FRotator(0.0f, 250.0f, 0.0f);
}

void UEnhancedMovementComponent::DecreaseSprintSpeed()
{
	// If the character is currently sprinting, set the sprinting flag to false to stop sprinting.
	// Speed adjustment factors. 
	float ScalingFactor = 5.0f * 0.25f; // A scaling factor for adjustments.
	float HalfFactor = 2.0f;			// Half the factor for calculations.
	float NearlyStoppedThreshold = ((Jog_MaxWalkSpeed / HalfFactor) / HalfFactor);  // Minimum velocity threshold for nearly stopped.
	float SprintToJogTransitionSpeed = Sprint_MaxWalkSpeed - 200.f;

	// Get the time interval since the last frame.
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();

	if (MaxWalkSpeed > Sprint_MaxWalkSpeed - (Jog_MaxWalkSpeed / HalfFactor))
	{
		MaxWalkSpeed = FMath::Lerp(MaxWalkSpeed, SprintToJogTransitionSpeed, DeltaSeconds);
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
#pragma endregion

#pragma region |Crouch|
void UEnhancedMovementComponent::SetCrouchInput(bool SetValue)
{
	bWantsToCrouch = SetValue;
}
#pragma endregion