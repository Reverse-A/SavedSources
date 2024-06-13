// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Synthetica/EnumTypes/CombatTypes.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Engine/Engine.h"
#include "InputActionValue.h"
#include "HUD/PlayerInterfaceManager.h"

#include "Interfaces/CrosshairInterface.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "BaseCharacter.generated.h"

#define ZERO 0.0f;

#define LOG_WARNING(Format)						UE_LOG(LogTemp, Warning, TEXT(Format));
#define LOG_WARNING_FLOAT(FloatValue)			UE_LOG(LogTemp, Warning, TEXT("%f"), FloatValue);
#define LOG_WARNING_TEXTANDFLOAT(Format, ...)	UE_LOG(LogTemp, Warning, TEXT(Format), __VA_ARGS__)

#define LINETRACE(World, StartPoint, EndPoint, Color) DrawDebugLine(World, StartPoint, EndPoint, Color);

#define ECC_SkeletalMesh ECollisionChannel::ECC_GameTraceChannel1

/*ClassCall*/
class UCameraComponent;
class UCoreAnimProcessor;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class USoundCue;

class APlayerControllerManager;
class APlayerStateManager;
class AWeapon;

UCLASS()
class SYNTHETICA_API ABaseCharacter : public ACharacter, public ICrosshairInterface
{
	GENERATED_BODY()

/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\--------------------------------\\*Synethica*//-------------------------------//----------------------------*/
/*------------------------------------------------------*[Base Character Comp.]*---------------------------------------------------------*/
/*-----------------------------------------\\Contains Information Related to Base Character//--------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY()	UCoreAnimProcessor*		  CoreAnimProcessor;
	UPROPERTY() APlayerControllerManager* PlayerControllerManager;
	UPROPERTY()	APlayerStateManager*	  PlayerStateManager;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void		 CastPlayerInputComponent();

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
//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
/*-----------------------------------------------------------------[Component]-----------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
private:
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UEnhancedMovementComponent* EnhancedMovement;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraSettings* CameraSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat Component", meta = (AllowPrivateAccess = "true"))
	class UCombatComponent*	Combat;

public:
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return Combat; }
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes", Replicated)	USkeletalMeshComponent* Head;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Torso;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Legs;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Arms;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Hands;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				USkeletalMeshComponent* Bag;


public:
/* Getter Appereance properties */
	FORCEINLINE USkeletalMeshComponent* GetHairMesh()	const { return Hair;  }
	FORCEINLINE USkeletalMeshComponent* GetHeadMesh()	const { return Head;  }
	FORCEINLINE USkeletalMeshComponent* GetTorsoMesh()  const { return Torso; }
	FORCEINLINE USkeletalMeshComponent* GetLegsMesh()	const { return Legs;  }
	FORCEINLINE USkeletalMeshComponent* GetArmsMesh()	const { return Arms;  }
	FORCEINLINE USkeletalMeshComponent* GetHandsMesh()	const { return Hands; }

	FORCEINLINE USkeletalMeshComponent* GetBagMesh()	const { return Bag;   }

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
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	UCameraComponent*	 ThirdPersonCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)	UCameraComponent*	 FirstPersonCamera;


	/* Getter And Setters*/
	FORCEINLINE USpringArmComponent*	GetThirdPersonSpringArm()	 { return ThirdPersonSpringArm; }
	FORCEINLINE UCameraComponent*		GetThirdPersonCamera() const { return ThirdPersonCamera;	}
	FORCEINLINE UCameraComponent*		GetFirstPersonCamera() const { return FirstPersonCamera;	}

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
UPROPERTY(EditAnywhere, Category = Input)			UInputMappingContext* MappingContext;

// List of action Inputs
UPROPERTY(EditAnywhere, Category = Input)			UInputAction* CameraSwitchAction;

UPROPERTY(EditAnywhere, Category = Input)			UInputAction* MoveInputAction;
UPROPERTY(EditAnywhere, Category = Input)			UInputAction* LookInputAction;

UPROPERTY(EditAnywhere, Category = Input)			UInputAction* InteractionAction;
UPROPERTY(EditAnywhere, Category = Input)			UInputAction* ReloadInputAction;
UPROPERTY(EditAnywhere, Category = Input)			UInputAction* AimInputAction;
UPROPERTY(EditAnywhere, Category = Input)			UInputAction* FireTriggerInputAction;


UPROPERTY(EditAnywhere, Category = Input)			UInputAction* JumpInputAction;
UPROPERTY(EditAnywhere, Category = Input)			UInputAction* CrouchInputAction;
UPROPERTY(EditAnywhere, Category = Input)			UInputAction* ProneInputAction;


UPROPERTY(EditAnywhere, Category = "Input Ability")	UInputAction* SprintInputAction;
UPROPERTY(EditAnywhere, Category = "Input Ability")	UInputAction* SlideInputAction;
UPROPERTY(EditAnywhere, Category = "Input Ability")	UInputAction* DashInputAction;

UPROPERTY(EditAnywhere, Category = "Miscellaneous") UInputAction* AutoRunToggleInputAction;

/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
/*--------------------------------------------------------------[Input Functions]--------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
#pragma region    | Camera |
void SwitchCamera_Input					(const FInputActionValue& Value);
#pragma endregion | Camera |

#pragma region   | MOVE - LOOK | 
/* For disabling Inputs */
private:
	UPROPERTY(Replicated)
	bool bDisableGameplay;

public:
	FORCEINLINE void DisableGameplay(bool SetValue) { bDisableGameplay = SetValue; }
	FORCEINLINE bool GameplayIsDisabled() const		{ return bDisableGameplay; }
#pragma region |MovementInput Directions Variables|
private:
	FVector2D MovementVector;

	void Movement_Input(const FInputActionValue& Value);
	void Movement_Input_Deactivation(const FInputActionValue& Value);

	UPROPERTY(Replicated)
	float MovementDirectionY;

	UPROPERTY(Replicated)
	float MovementDirectionX;

	UPROPERTY(Replicated)
	bool MovementInputActivated;

	float MovementInputDelayTimer;

	UFUNCTION(Server, Reliable)
	void Server_MovementDirectionValues(float MovementVectorX, float MovementVectorY);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MovementDirectionsValues(float MovementVectorX, float MovementVectorY);
	
	UFUNCTION(Server, Reliable)
	void Server_SetMovementInput(bool IsActive);

public:
	FORCEINLINE float GetMovementDirectionX()		const	{ return MovementDirectionX; }
	FORCEINLINE float GetMovementDirectionY()		const	{ return MovementDirectionY; }
	FORCEINLINE bool  GetMovementInputIsActivated() const	{ return MovementInputActivated; }
#pragma endregion

#pragma endregion


#pragma region		| INTERACTION |
	void Interaction_Input(const FInputActionValue& Value);
#pragma endregion

#pragma region		| Jump |

public:
	virtual void Jump() override;
	virtual void StopJumping() override;
	bool bCustomJump;

#pragma endregion 


#pragma region		| Sprint |
protected:
	void Sprint_InputActivation			(const FInputActionValue& Value);
	void Sprint_InputDeactivation		(const FInputActionValue& Value);
#pragma endregion	| Sprint | 


#pragma region		| Crouch |
	void CrouchInput_Activation			(const FInputActionValue& Value);
	void CrouchInput_Deactivation		(const FInputActionValue& Value);
	UPROPERTY(Replicated)				FVector CrouchTransformLocation;

	UFUNCTION(Server, Reliable)
	void Server_CrouchInput_Activation	(const FInputActionValue& Value);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Crouch(const FVector& NewLocation);
	UFUNCTION(Server, Reliable)
	void Server_CrouchInput_Deactivation(const FInputActionValue& Value);
#pragma endregion


#pragma region | Prone |
	void ProneInput						(const FInputActionValue& Value);
	void ProneInputReleased				(const FInputActionValue& Value);
	void PressButtonRestart();

	FTimerHandle ProneTimerHandle;

	bool ProneButtonAvaliable = true;
	bool ProneTimerGate;
#pragma endregion

#pragma region		| Mouse |
	void Look_Input(const FInputActionValue& Value);
#pragma endregion

#pragma region		| Weapon Types |
private:
	void AimButtonInput_Activation		(const FInputActionValue& Value);
	void AimButtonInput_Deactivation	(const FInputActionValue& Value);
	bool bCached_Aiming;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	void FireButtonInput_Activation 	(const FInputActionValue& Value);
	void FireButtonInput_Deactivation	(const FInputActionValue& Value);

	bool bCanFireIfDelayPassed = false;
	bool bFireButtonPressed;
	float bCanFireIfDelayPassTimer;

	void ReloadButtonInput(const FInputActionValue& Value);
public:
	bool IsAiming();


#pragma endregion

#pragma region		| Input Abilities |
private:
void SlideInput_Activation			(const FInputActionValue& Value);
void SlideInput_Deactivation		(const FInputActionValue& Value);

void DashInput_Activation			(const FInputActionValue& Value);
void DashInput_Deactivation			(const FInputActionValue& Value);
#pragma endregion

#pragma region |Miscellaneous Events via Keyboard|
bool bIsAutoRunEnabled;
bool CanAutoRun;
void AutoRun_Toggle();
public:
	void SetAutoRun(bool SetValue) { CanAutoRun = SetValue; }
	bool IsAutoRun() const { return CanAutoRun; }
#pragma endregion

#pragma region		| Crouch Input Variables|

#pragma endregion

#pragma region Getter And Setters 
/* Getters */

public:
	FORCEINLINE FVector2D		 GetMovementVector()				  const { return MovementVector; }
	FORCEINLINE UCameraSettings* GetCameraSettingsComponent()		  const { return CameraSettings; }

	//FORCEINLINE bool GetJumpPressed()						  const { return bCustomJumpPressed; }
/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
/*---------------------------------------------------------------[ NetWorking ]----------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	FCollisionQueryParams GetIgnoreCharacterParams() const;

#pragma endregion

#pragma region | Weapons |
/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
/*----------------------------------------------------------------[  Weapons  ]---------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
public:
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	void SetOverlappingWeapon(AWeapon* Weapon);

	AWeapon* GetEquippedWeapon();


#pragma endregion

#pragma region | Interfaces | 
/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
/*---------------------------------------------------------------[ Interfaces ]----------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/

/*
* Character's properties
*/
private:
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, EditAnywhere, Category = "Player Stats")
	float Health = 100.0f;

	UFUNCTION()
	void OnRep_Health();

public:
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

#pragma endregion

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
/*---------------------------------------------------------[ Damage && Hit && Health ]---------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
#pragma region | Damage && Hit |

public:
	void Elimination();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Elimination();
	void SetRagdollValues(bool SimulatePhysics, ECollisionEnabled::Type NewType);

	virtual void Destroyed() override;
protected:
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	void UpdateHUDHealth();

private:
	UPROPERTY(EditDefaultsOnly)
	float EliminationTimerDelay = 3.0f;

	FTimerHandle EliminationTimer;

	void EliminationTimerFinished();

// Dissolve Effect
private:
	UPROPERTY(VisibleAnywhere, Category = EliminationProperties)
	UTimelineComponent* DissolveTimeline;
	FOnTimelineFloat DissolveTrack;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UCurveFloat* DissolveCurve;

	// Dynamic instance that we can change at runtime
	UPROPERTY(VisibleAnywhere, Category = EliminationProperties)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// Material instance set on the bluepirnt, used with dynamic material instance
	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* DissolveHairMaterialInstance;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* DissolveHeadMaterialInstance;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UMaterialInstance* DissolveBodyMaterialInstance;

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();


// Death Section

private:
	bool bCharacterIsDead;

public:
	bool IsCharacterDead() const { return bCharacterIsDead; }
	void SetCharacterDead(bool SetStatus) { bCharacterIsDead = SetStatus; }

// EliminationBot
private:
	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	UParticleSystem* EliminationBotEffect;

	UPROPERTY(VisibleAnywhere, Category = EliminationProperties)
	UParticleSystemComponent* EliminationBotComponent;

	UPROPERTY(EditAnywhere, Category = EliminationProperties)
	USoundCue* EliminationBotSound;
/*
* Getter And Setters
*/
public:
	FVector GetHitTarget() const;
#pragma endregion

#pragma region | Other Getter And Setters |
/*---------------------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
/*-------------------------------------------------------------------[ HUD ]-------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
public:
	ECombatTypes GetCombatTypes() const;

protected:
	void PollInitialize();


#pragma endregion
};