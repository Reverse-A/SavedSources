// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Camera/ECameraPerspective.h"

#include "AnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API UAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UAnimationInstance();

	// Function called when the animation instance is initialized
	virtual void NativeInitializeAnimation() override;
	// Function called to update the animation instance each frame
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void Initialize();

protected:
	void AimOffset();
	void OffSetYawForLean(float DeltaTime, float Interpolate, float ClampNegative, float ClampPositive);

protected:
	// Other Classes and Components 
	UPROPERTY(BlueprintReadOnly, Category = Classes) class ABaseCharacter*			   BaseCharacter;
	UPROPERTY(BlueprintReadOnly, Category = Classes) class UEnhancedMovementComponent* EnhancedMovementComponent;
	UPROPERTY(BlueprintReadOnly, Category = Classes) class UCameraConfiguration*	   CameraConfigurationComponent;
	//UPROPERTY(BlueprintReadOnly, Category = Classes) class UCombatComponent*		   CombatComponent;

protected:
	// Enums
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) 
	ECameraViewPerspective CameraViewPerspective;

protected:
	/* "Variable calls from other classes" */ 
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float CurrentVelocity;

	/* Movement Variables */
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanSprint;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanCrouch;

	/* Input Properties */
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float HorizontalDirection;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float VerticalDirection;

	/* Aim Properties*/
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float AimOffsetPitch;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float AimOffsetYaw;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float YawDelta;


private:
	// AimOffset Rotators variables
	FRotator StartingAimRotation;

	// AimYawDelta Rotators variables
	FRotator RotationLastFrame;
	FRotator CharacterRotation;
};
