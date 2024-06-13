// @2023 "Age Of Reverse" Story. All rights reversed. 
// | Reverse - A Company.|

/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
// Copyright for Third - Party Users :																									 //
//																																		 //
// As a third - party user, it's essential to understand that "Synthetica" and "Age of Reverse" are the intellectual property			 //
// of Reverse-A, the game company that created them. Copyright law grants Reverse-A exclusive rights to control the use,reproduction,    //
// and distribution of these works.																										 //
//																																		 //
// This means that you do not have the legal right to use, copy, distribute, or adapt any part of "Synthetica" or "Age of Reverse"		 //
// without obtaining explicit permission or a licensing agreement from Reverse - A.Copyright infringement can result					 //
// in legal consequences, such as fines or lawsuits.																					 //
//																																		 //		
// Rights Reserved for Third - Party Users :																							 //	
// When Reverse - A states that "rights are reserved," it signifies their intention to enforce their copyright protections.				 //
// As a third - party user, you should respect these rights and seek permission															 //
// or licensing arrangements if you wish to use any aspect of their intellectual property.												 //
//																																		 //				
// If you have a specific request or proposal for using elements from "Synthetica" or "Age of Reverse,"									 //
// it's advisable to contact Reverse-A and discuss your intentions.																		 //
// They may consider licensing their intellectual property for certain purposes, but it ultimately depends on their policies and terms.  //
//																																		 //
// In summary, as a third - party user, you should be aware that "Synthetica" and "Age of Reverse" are protected by copyright,           //
// and you must obtain proper authorization from Reverse - A to use or incorporate any part of these works into your projects            //
// or endeavors. Respecting copyright laws and the rights reserved by the copyright holder is essential to avoid legal issues.           //
//																																		 //
//														   [Reverse-A]																	 //
//																																		 //
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<---*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<---*/
																							/*R*/										 
								/*H*/													/*F*/	/*E*/
							/*T*/	/*E*/											/*0*/			/*V*/
						/*N*/			/*T*/									/**/					/*E*/
					/*Y*/					/*I*/							/*E*/							/*R*/
				/*S*/							/*C*/					/*G*/									/*S*/
													/*A*/			/*A*/											/*E*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"


#include "BaseCharacter.generated.h"



#pragma region Macros
#define CharacterMesh GetMesh()

#define ZERO 0.0f;
#define LOG_WARNING(Format)						UE_LOG(LogTemp, Warning, TEXT(Format));
#define LOG_WARNING_FLOAT(FloatValue)			UE_LOG(LogTemp, Warning, TEXT("%f"), FloatValue);
#define LOG_WARNING_TEXTANDFLOAT(Format, ...)	UE_LOG(LogTemp, Warning, TEXT(Format), __VA_ARGS__)
#define LINETRACE(World, StartPoint, EndPoint, Color) DrawDebugLine(World, StartPoint, EndPoint, Color);

#define ECC_SkeletalMesh	ECollisionChannel::ECC_GameTraceChannel1
#define ECC_VisibilityBlock ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block

#pragma endregion

#pragma region Pre-call Classes
class AWeapon;

class UCameraComponent;
class UAnimationInstance;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class USoundCue;
#pragma endregion

UCLASS()
class SYNTHETICA_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\--------------------------------\\*Synethica*//-------------------------------//----------------------------*/
/*------------------------------------------------------*[Base Character Comp.]*---------------------------------------------------------*/
/*-------------------------------------------------------\\General Functions//-----------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
#pragma region General Functionalities 
public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	void CastComponentClasses();

public:
	FCollisionQueryParams GetIgnoreCharacterParams() const;

private:
	UPROPERTY()	UAnimationInstance*			AnimationInstance;
	//UPROPERTY() APlayerControllerManager*	PlayerControllerManager;
	//UPROPERTY()	APlayerStateManager*		PlayerStateManager;

#pragma endregion

#pragma region Component Classes
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<---*/
																							/*R*/
								/*H*/													/*F*/	/*E*/
							/*T*/	/*E*/											/*0*/			/*V*/
						/*N*/			/*T*/									/**/					/*E*/
					/*Y*/					/*I*/							/*E*/							/*R*/
				/*S*/							/*C*/					/*G*/									/*S*/
													/*A*/			/*A*/											/*E*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
/*-------------------------------------------------------------[Class Components]--------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
private:
	UPROPERTY(VisibleAnywhere, Category = CustomComponents)
	class UEnhancedMovementComponent* EnhancedMovementComponent;

	UPROPERTY(VisibleAnywhere,  Category = CustomComponents)
	class UCameraConfiguration*		  CameraConfigurationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat Component", meta = (AllowPrivateAccess = "true"))
	class UCombatComponent*			  Combat;
public:
	FORCEINLINE UEnhancedMovementComponent* GetEnhancedMovementComponent()    const { return EnhancedMovementComponent; }
	FORCEINLINE UCameraConfiguration*		GetCameraConfigurationComponent() const { return CameraConfigurationComponent; }
	FORCEINLINE UCombatComponent*			GetCombatComponent()			  const { return Combat; }

#pragma endregion

#pragma region Character Appearance
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<---*/
																							/*R*/
								/*H*/													/*F*/	/*E*/
							/*T*/	/*E*/											/*0*/			/*V*/
						/*N*/			/*T*/									/**/					/*E*/
					/*Y*/					/*I*/							/*E*/							/*R*/
				/*S*/							/*C*/					/*G*/									/*S*/
													/*A*/			/*A*/											/*E*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
/*-----------------------------------------------------[Character's Apperance Components]------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------These member variables represent the various meshes for the character's appearance and equipment.---------------------*/
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Hair;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Head;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Legs;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Arms;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Hands;

	FORCEINLINE USkeletalMeshComponent* GetHairMesh()	const { return Hair; }
	FORCEINLINE USkeletalMeshComponent* GetHeadMesh()	const { return Head; }
	FORCEINLINE USkeletalMeshComponent* GetLegsMesh()	const { return Legs; }
	FORCEINLINE USkeletalMeshComponent* GetArmsMesh()	const { return Arms; }
	FORCEINLINE USkeletalMeshComponent* GetHandsMesh()	const { return Hands; }
#pragma endregion

#pragma region Camera
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<---*/
																							/*R*/
								/*H*/													/*F*/	/*E*/
							/*T*/	/*E*/											/*0*/			/*V*/
						/*N*/			/*T*/									/**/					/*E*/
					/*Y*/					/*I*/							/*E*/							/*R*/
				/*S*/							/*C*/					/*G*/									/*S*/
													/*A*/			/*A*/											/*E*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
/*-------------------------------------------------------------[Camera Components]-------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	USpringArmComponent* ThirdPersonSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	UCameraComponent*	 ThirdPersonCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	UCameraComponent*	 FirstPersonCamera;

/* Getter And Setters*/
public:
	FORCEINLINE USpringArmComponent*	GetThirdPersonSpringArm()	 { return ThirdPersonSpringArm; }
	FORCEINLINE UCameraComponent*		GetThirdPersonCamera() const { return ThirdPersonCamera;	}
	FORCEINLINE UCameraComponent*		GetFirstPersonCamera() const { return FirstPersonCamera;	}
#pragma endregion

#pragma region Input
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<----\\//---->>>><<<<---*/
																							/*R*/
								/*H*/													/*F*/	/*E*/
							/*T*/	/*E*/											/*0*/			/*V*/
						/*N*/			/*T*/									/**/					/*E*/
					/*Y*/					/*I*/							/*E*/							/*R*/
				/*S*/							/*C*/					/*G*/									/*S*/
													/*A*/			/*A*/											/*E*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
/*-------------------------------------------------------------[Input Properties]--------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
// This section contains member variables related to character input handling.
// It includes mappings, actions, and other input-related configurations.

protected:
// MappingContext 
// The input mapping context that holds the mapping configurations for this character's input.
UPROPERTY(EditAnywhere, Category = InputSettings)			UInputMappingContext* MappingContext;

// List of action Inputs
UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* CameraSwitchAction;

UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* MoveInputAction;
UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* LookInputAction;

UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* InteractionInputAction;
UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* ReloadInputAction;
UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* AimInputAction;
UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* FireInputAction;


UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* JumpInputAction;
UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* CrouchInputAction;
UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* ProneInputAction;

UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* SprintInputAction;
UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* SlideInputAction;
UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* DashInputAction;

UPROPERTY(EditAnywhere, Category = InputSettings)			UInputAction* AutoRunInputAction;
/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
/*--------------------------------------------------------------[Input Functions]--------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
private:
	void EnhancedInputLocalPlayerSubsystem();

	void InputSwitchCamera				(const FInputActionValue& Value);

	void InputMovement_Activation		(const FInputActionValue& Value);
	void InputMovement_DeActivation		(const FInputActionValue& Value);

	void InputLook						(const FInputActionValue& Value);

	void InputInteraction				(const FInputActionValue& Value);
	
	void InputReload					(const FInputActionValue& Value);
	void InputAim_Activation			(const FInputActionValue& Value);
	void InputAim_DeActivation			(const FInputActionValue& Value);
	void InputFire						(const FInputActionValue& Value);

	void InputJump						(const FInputActionValue& Value);
	void InputCrouch_Activation			(const FInputActionValue& Value);
	void InputCrouch_DeActivation		(const FInputActionValue& Value);

	void InputProne						(const FInputActionValue& Value);
	void InputSprint					(const FInputActionValue& Value);
	void InputSlide						(const FInputActionValue& Value);
	void InputDash						(const FInputActionValue& Value);

	void InputAutoRun					(const FInputActionValue& Value);

/* Input functions variables */
private:
	FVector2D MovementVector;

	//UPROPERTY(Replicated)
	float MovementDirectionY;
	//UPROPERTY(Replicated)
	float MovementDirectionX;
	//UPROPERTY(Replicated)
	bool MovementInputActivated
		;
public:
	FORCEINLINE float GetMovementDirectionX()		const { return MovementDirectionX; }
	FORCEINLINE float GetMovementDirectionY()		const { return MovementDirectionY; }
	FORCEINLINE bool  GetMovementInputIsActivated() const { return MovementInputActivated; }
#pragma endregion

#pragma region Aim Properties
private:
	bool bAiming = false;
	bool bCached_Aiming;

	bool CanAim();

public:
	FORCEINLINE bool IsAiming() const { return bAiming; }
	FORCEINLINE void SetAim(bool SetValue) { bAiming = SetValue; }
#pragma endregion


#pragma region OptionMenu
private:
	UPROPERTY(EditAnywhere, Category = Options)
	float MouseSensitivity;

public:
	FORCEINLINE float GetMouseSensitivityValue() const			{ return MouseSensitivity; }
	FORCEINLINE void  SetMouseSensitivityValue(float SetValue)	{ MouseSensitivity = SetValue; }
#pragma endregion

#pragma region Weapon
public:
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	void SetOverlappingWeapon(AWeapon* Weapon);

	AWeapon* GetEquippedWeapon();
#pragma endregion
};
