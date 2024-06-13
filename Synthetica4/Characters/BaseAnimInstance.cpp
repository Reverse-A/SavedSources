// ©2023 Reverse-A. All rights reserved.

#include "BaseAnimInstance.h"
#include "BaseCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Synthetica/Weapons/Weapon.h"
#include "Synthetica/Weapons/RangedWeaponTypes.h"
#include "Kismet/KismetMathLibrary.h"



void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());

	bShouldDoIK = false;

}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	// This function, NativeUpdateAnimation, is an overridden function in an animation instance class.
	// It is responsible for updating the animation parameters and variables used in the animation blueprint.

	// Call the base class's implementation of the function
	Super::NativeUpdateAnimation(DeltaTime);

	// Store the delta time for later use as variable
	DeltaTimeStore = DeltaTime; 

	// Check if the BaseCharacter reference is null and attempt to retrieve it from the owning pawn
	if (BaseCharacter == nullptr)
	{
		BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	}

	// If the BaseCharacter is still null, exit the function
	if (BaseCharacter == nullptr) return;


	// Update various animation-related variables and parameters from the BaseCharacter
	if (BaseCharacter)
	{
		// Update character state
		CurrentCharacterState = BaseCharacter->GetCurrentCharacterState();

		// Update slope pitch degree angle
		SlopePitchDegreeAngle = BaseCharacter->GetSlopePitchDegreeAngle();

		// Update movement speed
		MovementSpeed = BaseCharacter->GetVelocity().Size();

		bCharacterIsMoving = BaseCharacter->GetIsMoving();

		// Update acceleration status
		bIsAccelerating = BaseCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0 ? true : false;

		// Update in-air status
		bIsInAir = BaseCharacter->GetCharacterMovement()->IsFalling();

		bIsJumping = BaseCharacter->GetIsJumping();

		bIsNormalJump = BaseCharacter->GetIsNormalJumping();

		bSprintJump = BaseCharacter->GetSprintJump();

		// Update aiming status
		bAiming = BaseCharacter->IsAiming();

		// Update crouch status
		bIsCrouched = BaseCharacter->GetCrouch();

		// Update sprinting status
		bIsSprinting = BaseCharacter->GetSprinting();

		// Update weapon-equipped status
		bWeaponEquipped = BaseCharacter->IsWeaponEquipped();

		// Update current weapon class
		WeaponClass = BaseCharacter->GetEquippedWeapon();

		// Update character rotation state for in-place rotation
		RotateStatementEnum = BaseCharacter->GetRotateCharacterInPlace();

		// Update idle status
		bIsIdle = MovementSpeed == 0;

		// Update reloading status
		bIsReloading = BaseCharacter->GetIsReloading();

		CharacterIsFalling = BaseCharacter->GetCanFall();

		// Update additive overlay yaw and pitch values
		AO_Yaw = BaseCharacter->GetAO_Yaw();
		AO_Pitch = BaseCharacter->GetAO_Pitch();

		// Update movement inputs
		Movement_X = BaseCharacter->GetMovementX();
		Movement_Y = BaseCharacter->GetMovementY();

		// Update difference between the feet for foot IK
		DifferenceBetweenTwoFeet = BaseCharacter->GetDifferenceBetweenTwoFeet();

		// Update distance to the ground for foot IK
		LeftDistanceToGround = BaseCharacter->GetLeftDistanceToGround();
		RightDistanceToGround = BaseCharacter->GetRightDistanceToGround();

		// Update grenade throwing status
		bGrenadeThrowning = BaseCharacter->GetThrowGrenade();

		HoldingWall = BaseCharacter->GetIsHoldingWall(); 

		bIsGround = BaseCharacter->GetCharacterMovement()->IsMovingOnGround();

		// Update ranged weapon types (commented out until animations are properly set for each weapon type)
		if (bWeaponEquipped) RangeWeaponTypes = BaseCharacter->GetEquippedWeapon()->GetRangedWeaponTypes();

	}

	// Update animation variables based on aiming state
	if (bAiming)
	{
		PitchYawRollReturn(DeltaTime);
		YawForAimCalculation(DeltaTime);
	}

	// Update animation variables for character leaning
	OffSetYawForLean(DeltaTime, 6.f, -45.f, 45.f);

	// Update animation variables for the weapon's position and rotation
	if (!bIsReloading ||
		!bGrenadeThrowning ||
		BaseCharacter->GetEquippedWeapon()->GetRangedWeaponTypes() != ERangedWeaponTypes::ERWT_Pistol)
		FabrikMethod();

	// Update foot IK animation variables based on character movement
	if (MovementSpeed == 0)	ActiveFootIK(DeltaTime);
	else DisableFootIK();

	// Update progress for foot IK animation
	FootIKProgress(DeltaTime);
}


void UBaseAnimInstance::SelectSocketName()
{
	if (BaseCharacter->GetEquippedWeapon())
	{
		ERangedWeaponTypes SwitchSocketNameByGetRangeWeaponType;
		SwitchSocketNameByGetRangeWeaponType = BaseCharacter->GetEquippedWeapon()->GetRangedWeaponTypes();
		switch (SwitchSocketNameByGetRangeWeaponType)
		{
		case ERangedWeaponTypes::ERWT_Pistol:
			LeftHandSocketName = "LeftHandPistolSocketName";
			LeftHandSocketNameAim = "LeftHandPistolSocketNameAim";
			break;
		case ERangedWeaponTypes::ERWT_AssaultRifle:
			LeftHandSocketName = "LeftHandRifleSocket";
			LeftHandSocketNameAim = "LeftHandRifleSocketAim";
			LefHandSocketNameWhileSprinting = "LeftHandRifleSocketSprinting";
			break;
		}
	}

}

void UBaseAnimInstance::FabrikMethod()
{
	if(BaseCharacter->GetAO_Yaw() <= -89.9f || BaseCharacter->GetAO_Yaw() >= 89.9f)
	{
		if (bDoFabrik)
			bDoFabrik = false;
	} 
	else
	{
		if (!bDoFabrik)
			bDoFabrik = true;
	}

	if (!bDoFabrik) return;

	if (bDoFabrik)
	{
		SelectSocketName();
		// Fabrik method: This is a method designed for any weapon to follow the socket we specify on the other hand bone on that location acccording to the transform.

		// Check if the weapon is equipped and valid, and if the character's mesh is valid
		if (bWeaponEquipped && WeaponClass && WeaponClass->GetWeaponMesh() && BaseCharacter->GetMesh() && bDoFabrik)
		{
			// Determine the transform for the left hand based on whether the character is aiming or not
			SelectSocketName();

			if (bAiming)
			{
				LeftHandTransform = WeaponClass->GetWeaponMesh()->GetSocketTransform(LeftHandSocketNameAim, ERelativeTransformSpace::RTS_World);
			}
			else
			{
				LeftHandTransform = WeaponClass->GetWeaponMesh()->GetSocketTransform(LeftHandSocketName, ERelativeTransformSpace::RTS_World);

			}

			if (bIsSprinting)
			{
				LeftHandTransform = WeaponClass->GetWeaponMesh()->GetSocketTransform(LefHandSocketNameWhileSprinting, ERelativeTransformSpace::RTS_World);
			}

			// Transform the left hand's location and rotation to the bone space of the "hand_r" bone

			FVector OutPosition;
			FRotator OutRotation;
			BaseCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
			LeftHandTransform.SetLocation(OutPosition);
			LeftHandTransform.SetRotation(FQuat(OutRotation));


			// If the character is locally controlled, calculate the rotation for the right hand to look at a target
			if (BaseCharacter->IsLocallyControlled())
			{
				FTransform RightHandTransform = WeaponClass->GetWeaponMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BaseCharacter->GetHitTarget()));
				RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTimeStore, 30.f);
			}
		}
	} 
}

void UBaseAnimInstance::PitchYawRollReturn(float DeltaTime)
{
	// Calculates the Pitch, Yaw, and Roll of the character based on the difference between the Base Aim Rotation and the Actor Rotation.

	// Get the current aim rotation of the character
	FRotator AimRotation = BaseCharacter->GetBaseAimRotation();

	// Get the current rotation of the character
	FRotator ActorRotation = BaseCharacter->GetActorRotation();

	// Calculate the difference in rotation between the aim rotation and the actor rotation
	DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);


	// Extract the roll, pitch, and yaw values from the delta rotation
	Roll  = DeltaRotation.Roll;
	Pitch = DeltaRotation.Pitch;
	Yaw   = DeltaRotation.Yaw;
}

void UBaseAnimInstance::OffSetYawForLean(float DeltaTime,float Interpolate, float ClampNegative, float ClampPositive)
{
	// This function calculates the offset yaw for the lean animation.
	// It uses the difference between the character's current rotation and its rotation in the previous frame to calculate the yaw delta.
	// The yaw delta is then interpolated to create a smooth transition between the previous and current values.
	// The result is clamped between -90 and 90 to prevent the character from leaning too far in any direction.

	// Store the rotation of the character in the previous frame
	RotationLastFrame = CharacterRotation;

	// Get the current rotation of the character
	CharacterRotation = BaseCharacter->GetActorRotation();

	// Calculate the difference in rotation between the current frame and the previous frame
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, RotationLastFrame);

	// Calculate the yaw delta per second
	const float Target = Delta.Yaw / DeltaTime;

	// Interpolate the yaw delta to create a smooth transition between the previous and current values
	const float Interp = FMath::FInterpTo(YawDelta, Target, DeltaTime, Interpolate);

	// Clamp the interpolated value between the specified negative and positive limits
	YawDelta = FMath::Clamp(Interp, ClampNegative, ClampPositive);
}




void UBaseAnimInstance::YawForAimCalculation(float DeltaTime)
{
	//This function is used to calculate the yaw offset for aim calculation in an animation instance.
	//It takes into account the aim rotation of the character and the movement rotation to determine the delta rotation.
	//The delta rotation is then interpolated over time to create a smooth transition.
	//The resulting yaw offset is used for aim - related animations.

	// Get the aim rotation of the character
	FRotator AimRotation = BaseCharacter->GetBaseAimRotation();

	// Get the movement rotation of the character based on its velocity
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BaseCharacter->GetVelocity());

	// Calculate the delta rotation between the movement rotation and aim rotation
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);

	// Interpolate the delta rotation over time to create a smooth transition
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);

	// Extract the yaw offset from the delta rotation
	YawOffset = DeltaRotation.Yaw;
}

void UBaseAnimInstance::ActiveFootIK(float DeltaTime)
{
	// This function is used to activate foot inverse kinematics(IK) in an animation instance.
	// It determines whether foot IK should be enabled or disabled based on various conditions 
	// related to the character's position in relation to the ground.

	// Check if the character is in the air, and if so, return and skip foot IK activation
	if (bIsInAir == true) 
		return;

	// Check if the distance to the ground for either foot is greater than 5 units
	if (LeftDistanceToGround > 5.f ||
	   RightDistanceToGround > 5.f)
	{

		// Increment the flagged time lapse
		FlaggedTimeLapse += DeltaTime;

		// Check if the flagged time lapse has exceeded the threshold
		if (FlaggedTimeLapse > .5f)
		{
			// Set the IK type to 'Process' if it is not already
			if (IKType != EIKType::EIKT_Process) 
				IKType = EIKType::EIKT_Process;

			// Enable foot IK
			if(!bShouldDoIK) 
				bShouldDoIK = true;
			
		}
	}
	// Check if the distance to the ground for either foot is less than 5 units and the difference between the two feet is less than 1 unit
	else if (LeftDistanceToGround < 5.f && DifferenceBetweenTwoFeet < 1.f ||
			RightDistanceToGround < 5.f && DifferenceBetweenTwoFeet < 1.f)
	{
		// Set the IK type to 'Leave' if it is not already
		if (IKType != EIKType::EIKT_Leave)
			IKType = EIKType::EIKT_Leave;

		// Disable foot IK
		if (bShouldDoIK)
			bShouldDoIK = false;

		// Reset the flagged time lapse
		if (FlaggedTimeLapse != 0) 
			FlaggedTimeLapse = 0.f;
	}
}


void UBaseAnimInstance::FootIKProgress(float DeltaTime)
{
	// This function is used to control the progress of the foot inverse kinematics (IK) animation. 
	// It determines whether the IK animation should progress or reset based on the value of bShouldDoIK.

	// Check if foot IK should be performed
	if (bShouldDoIK)
	{
		// Check if the IK alpha value has reached the maximum value (near 1.0)
		if (IKAlphaValue >= 0.95f) 
			return;

		// If the IK alpha value is less than 1.0, interpolate it towards 1.0 to progress the IK animation
		if (IKAlphaValue < 1.f) IKAlphaValue = FMath::Lerp(IKAlphaValue, 1.f, 1.5f * (DeltaTime / 2.f));
	}
	else
	{
		// Check if the IK alpha value is already at 0.0
		if (IKAlphaValue == 0.f)
			return;

		// Reset the IK alpha value to 0.0 to restart the IK animation
		IKAlphaValue = 0.f;
	}
}

void UBaseAnimInstance::DisableFootIK()
{
	// This function is used to disable the foot inverse kinematics (IK) animation and reset relevant variables.
	// It ensures that the IK animation is set to the "leave" state, clears the flagged time lapse, and sets bShouldDoIK to false.

	// Check if the IK type is not already set to "leave"
	if (IKType != EIKType::EIKT_Leave)
		IKType = EIKType::EIKT_Leave;

	// Reset the flagged time lapse to 0.0
	if(FlaggedTimeLapse != 0) 
		FlaggedTimeLapse = 0.f;

	// Disable foot IK by setting bShouldDoIK to false
	if (bShouldDoIK)
		bShouldDoIK = false;
}
