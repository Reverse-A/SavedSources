// ©2023 Reverse - A Company.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/Engine.h"
#include "InputActionValue.h"
#include "Camera/CustomCameraComponent.h"

#include "BaseCharacter.generated.h"

#define ZERO 0.0f;
#define LOG_WARNING(Format)						UE_LOG(LogTemp, Warning, TEXT(Format));
#define LOG_WARNING_FLOAT(FloatValue)			UE_LOG(LogTemp, Warning, TEXT("%f"), FloatValue);
#define LOG_WARNING_TEXTANDFLOAT(Format, ...)	UE_LOG(LogTemp, Warning, TEXT(Format), __VA_ARGS__)

#define LINETRACE(World, StartPoint, EndPoint, Color) DrawDebugLine(World, StartPoint, EndPoint, Color);

/*First-Call for classes*/
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UBaseCharacterMovementComponent;

UCLASS()
class SYNTHETICA_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*-----------------------------------------------*[Base Character Comp.]*-------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------\\Contains Information Related to Base Character//--------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
virtual void BeginPlay() override;
virtual void Tick(float DeltaTime) override;
virtual void PostInitializeComponents() override;

/*-----------[GeneralFunctions #End]--------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*-----------[GeneralVariables]-------------------------------------------------------------------------------------------*/

float CanSlideTimer;
/*-----------[GeneralVariables #End]--------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/



																			/*R*/
				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//-------------------------/*Synethica*///------------------------//---------------------------*/
/*-------------------------------------------*[Character's Helper Components]*--------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement) 
	UBaseCharacterMovementComponent* BaseCharacterMovementComponent;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCustomCameraComponent* CustomCameraComponent;
/*------------------------------------------------------------------------------------------------------------------------*/


																			/*R*/
				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//-------------------------/*Synethica*///------------------------//---------------------------*/
/*-----------------------------------------*[Character's Apperance Components]*-------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*---------These member variables represent the various meshes for the character's appearance and equipment.--------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
public:
UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Hair; 
UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Head;
UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Torso;
UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Legs;
UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Arms;
UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Hands;


FORCEINLINE USkeletalMeshComponent* GetHeadMesh() const { return Head; }
/*------------------------------------------------------------------------------------------------------------------------*/




#pragma region Controller[{Input}]
/*------------------------------------------------------------------------------------------------------------------------*/
																			/*R*/
				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/

							/**/				/*Synethica*/					/**/
											/*Input Components*/
/*------------------------------------------------------------------------------------------------------------------------*/
// This section contains member variables related to character input handling.
// It includes mappings, actions, and other input-related configurations.
protected:
// MappingContext 
// The input mapping context that holds the mapping configurations for this character's input.
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputMapping)	UInputMappingContext* MappingContext;

// List of action Inputs
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")			UInputAction* CameraSwitchAction;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")			UInputAction* MoveInputAction;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")			UInputAction* LookInputAction;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")			UInputAction* JumpInputAction;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")			UInputAction* CrouchInputAction;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")			UInputAction* ProneInputAction;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Ability")	UInputAction* SprintInputAction;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Ability")	UInputAction* SlideInputAction;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Ability")	UInputAction* AimInputAction;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Ability")	UInputAction* DashInputAction;
/*------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------//-------------------------\\*Synethica*//*------------------------*//*------------------------*/
/*--------------------------------------------------------*[Jump]*--------------------------------------------------------*/
public:
	virtual void Jump() override;
	virtual void StopJumping() override;

public:
	bool bCustomJumpPressed;

/*------------------------------------------------------------------------------------------------------------------------*/


//	List of Input Functionalities 
//	Basicly, actions to be invoked by simply pressing, releasing or holding the key
virtual void SetupPlayerInputComponent	(class UInputComponent* PlayerInputComponent) override;

void SwitchCameraInput					(const FInputActionValue& Value);

void SprintInput_Activate				(const FInputActionValue& Value);
void SprintInput_Deactivate				(const FInputActionValue& Value);
void AimButtonInput_Activate			(const FInputActionValue& Value);
void AimButtonInput_Deactivate			(const FInputActionValue& Value);

void MovementInput_Activate				(const FInputActionValue& Value);
void MovementInput_Deactivate			(const FInputActionValue& Value);
void LookInput							(const FInputActionValue& Value);
void JumpInput							(const FInputActionValue& Value);

void CrouchInput_Activate				(const FInputActionValue& Value);
void CrouchInput_Deactivate				(const FInputActionValue& Value);
void ProneInput_Toggle					(const FInputActionValue& Value);

// Ability
void SlideInput_Activate				(const FInputActionValue& Value);
void SlideInput_Deactivate				(const FInputActionValue& Value);

void DashInput_Activate					(const FInputActionValue& Value);
void DashInput_Deactivate				(const FInputActionValue& Value);


// Movement inputs variables
bool bIsMovementInputActive;

/*------------------------------------------------------------------------------------------------------------------------*/
#pragma endregion

#pragma region Camera Perspective
/*------------------------------------------------------------------------------------------------------------------------*/
																			/*R*/
				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------//-------------------------\\*Synethica*//*------------------------*//*------------------------*/
/*----------------------------------------------------*[Perspective]*-----------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	USpringArmComponent* ThirdPersonSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	UCameraComponent* ThirdPersonCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	UCameraComponent* FirstPersonCamera;

	//Region |CameraSetup|
public:
	void CalculateAndClampAimOffset();

protected:
	void ThirdPersonPerspective(); 
	void FirstPersonPerspective();

private:
	// Rotation
	bool IsTurningRight;
	bool IsTurningLeft;

	// FPP Moving Direction
	bool isMovingForward;
	bool isMovingBackward;
	bool isMovingRight;
	bool isMovingLeft;

	float HorizontalDirectionsValue_FPP;
	float VerticalDirectionsValue_FPP;

private: //AimOffset
	float AimOffSet_Yaw;
	float AimOffSet_Pitch;
#pragma endregion
/*------------------------------------------------------------------------------------------------------------------------*/

																			/*R*/
				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//-------------------------\\*Synethica*//------------------------//---------------------------*/
/*------------------------------------------------------*[Getters]*-------------------------------------------------------*/
public:
	FCollisionQueryParams GetIgnoreCharacterParams() const;

public:
	// Camera Components
	// Returns the Spring Arm component used in the third-person perspective.
	FORCEINLINE USpringArmComponent* GetBaseCharacterSpringArm() const { return ThirdPersonSpringArm; }

	// Returns the third-person camera component.
	FORCEINLINE UCameraComponent* GetThirdPersonCamera()		 const { return ThirdPersonCamera; }
	// Returns the first-person camera component.
	FORCEINLINE UCameraComponent* GetFirstPersonCamera()		 const { return FirstPersonCamera; }

	// Sets whether the character is turning right.
	FORCEINLINE bool SetTurningRight(bool Value)		{ return IsTurningRight = Value; }
	// Sets whether the character is turning left.
	FORCEINLINE bool SetTurningLeft(bool Value)			{ return IsTurningLeft = Value; }
	// Returns whether the character is currently turning right.
	FORCEINLINE bool GetTurningRight() const			{ return IsTurningRight; }
	// Returns whether the character is currently turning left.
	FORCEINLINE bool GetTurningLeft()  const			{ return IsTurningLeft; }

	/* For First-Person Perspective */
	// Returns the horizontally directed input value in first-person perspective.
	FORCEINLINE float GetHorizontallyDirections_FPP()	const { return HorizontalDirectionsValue_FPP; }
	// Returns the vertically directed input value in first-person perspective.
	FORCEINLINE float GetVerticallyDirections_FPP()		const { return VerticalDirectionsValue_FPP; }

	// Returns whether the character is currently moving forward.
	FORCEINLINE bool GetCharacterIsMovingForward()	const { return isMovingForward; }
	// Returns whether the character is currently moving backward.
	FORCEINLINE bool GetCharacterIsMovingBackward() const { return isMovingBackward; }
	// Returns whether the character is currently moving right.
	FORCEINLINE bool GetCharacterIsMovingRight()	const { return isMovingRight; }
	// Returns whether the character is currently moving left.
	FORCEINLINE bool GetCharacterIsMovingLeft()		const { return isMovingLeft; }

	/* Aim Offset */
	// Returns the yaw angle of the aim offset.
	FORCEINLINE float GetAimOffSetYaw()				const	{ return AimOffSet_Yaw; }
	// Sets the yaw angle of the aim offset.
	FORCEINLINE float SetAimOffSetYawValue(float Value)		{ return AimOffSet_Yaw = Value; }
	// Returns the pitch angle of the aim offset.
	FORCEINLINE float GetAimOffSetPitch()	const			{ return AimOffSet_Pitch; }
	// Sets the pitch angle of the aim offset.
	FORCEINLINE float SetAimOffSetPitchValue(float Value)	{ return AimOffSet_Pitch = Value; }


/* Input Variables */
	// Returns whether movement input is currently active.
	FORCEINLINE bool GetMovementInputActive()		const { return bIsMovementInputActive; }
	// Returns whether the character can jump.
	FORCEINLINE bool GetJumpPressed()				const { return bCustomJumpPressed; }
};
