// ©2023 Reverse - A Company.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Camera/CustomCameraComponent.h"


#include "BaseAnimInstance.generated.h"

UCLASS()
class SYNTHETICA_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
/*------------------------------------------------------------------------------------------------------------------------*/


																			/*R*/
				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*---------------------------------------------------*[Constructor]*------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
public:
	//UBaseAnimInstance(const FObjectInitializer& ObjectInitializer); De-Activated
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[General Functions]*---------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
	// Function called when the animation instance is initialized
	virtual void NativeInitializeAnimation() override;

	// Function called to update the animation instance each frame
	virtual void NativeUpdateAnimation(float DeltaTime) override;
/*------------------------------------------------------------------------------------------------------------------------*/
																			/*R*/
				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*--------------------------------------------------*[Other Classes]*-----------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
	UPROPERTY(BlueprintReadOnly, Category = Character)	class ABaseCharacter* BaseCharacter;

	class UBaseCharacterMovementComponent* BaseCharacterMovementComponent;
	class UBaseAnimInstance* BaseAnimInstance;

/*------------------------------------------------------------------------------------------------------------------------*/


																			/*R*/
				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*----------------------------------------------------*[Functions]*-------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
protected:
	bool BaseCharacterIsValid();
	void UpdateCharacterAnimationProperties();
	virtual void OffSetYawForLean(float DeltaTime, float Interpolate, float ClampNegative, float ClampPositive);
/*------------------------------------------------------------------------------------------------------------------------*/


																			/*R*/
				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*----------------------------------------------------*[Variables]*-------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
protected:
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) ECameraViewPerspective CameraViewPerspective;


protected:
	// Current lenght( magnitude) of Velocity from Movement Component
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float VelocitySize;
	// It represents the positive and negative value of the character in the yaw axis compared to the forward vector.
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float YawDelta;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float AimOffset_Pitch;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float AimOffset_Yaw;


	/*Input*/
	/*
	* If at the keyboard "WASD" input value is triggered this value will true
	* Else if Inputs are released later it will take false value.
	*/ 
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool MovementInputIsActive;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanCrouch;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanSprint;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanProne;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanJump;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CanSlide;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool IsInAir;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool IsOnGround;

	// Character Rotati
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool IsTurningRight;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool IsTurningLeft;

	// Wall-Run Statement
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool IsWallRunning;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool IsWallRunningRight;

	// First person perspective Directions
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float HorizontallyDirectionsValue_FPP;
	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) float VerticallyDirectionsValue_FPP;

	// Reserved for Future uses
	//UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CharacterMovingForward;
	//UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CharacterMovingBackward;
	//UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CharacterMovingRight;
	//UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) bool CharacterMovingLeft;


//	Yaw Delta
private:
	float YawDeltaResetTimer;

	float fixFrameRefreshTime;
	// Last frame rotation of the character
	FRotator RotationLastFrame;

	// Rotation of the character
	FRotator CharacterRotation;

public:
	FORCEINLINE float GetYawDelta() const { return YawDelta; }
	FORCEINLINE UBaseAnimInstance* GetBaseAnimInstanceClass() const { return BaseAnimInstance; }

};
