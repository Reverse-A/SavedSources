// ©2023 Reverse - A Company.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacter.h"

#include "BaseCharacterMovementComponent.generated.h"

class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashStartDelegate);

#pragma region Custom Movement Mode - ENUM
UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Sprint		UMETA(DisplayName = "Sprint"	),		
	CMOVE_Crouch		UMETA(DisplayName = "Crouch"	),
	CMOVE_Prone			UMETA(DisplayName = "Prone"		),
	CMOVE_Slide			UMETA(DisplayName = "Slide"		),
	CMOVE_Dash			UMETA(DisplayName = "Dash"		),
	CMOVE_WallRun		UMETA(DisplayName = "Wall Run"	),
	CMOVE_Hang			UMETA(DisplayName = "Hang"		),
	CMOVE_Climb			UMETA(DisplayName = "Climb"		),
	CMOVE_MAX			UMETA(Hidden),
};
#pragma endregion

#pragma region BaseCharacterMovementComponent Class
UCLASS()
class SYNTHETICA_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
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
//-------------------------//-------------------------\\*Synethica*//------------------------//---------------------------*/
/*-----------------------------------------------*[SavedMove FrameWork]*--------------------------------------------------*/
class FSavedMove_Synthetica : public FSavedMove_Character
{
	typedef FSavedMove_Character Super;

public:
	enum CompressedFlags
	{
		FLAG_Sprint		= 0x1,
		FLAG_Crouch		= 0x2,
		FLAG_Slide		= 0x4,
		FLAG_Prone		= 0x8,
		FLAG_Dash		= 0x10,
		FLAG_WallRun	= 0x20,
		FLAG_Hang		= 0x40,
		FLAG_Climb		= 0x80,
	};

	uint8 Saved_bWantsToSprint		: 1;
	uint8 Saved_bWantsToCrouch		: 1;
	uint8 Saved_bWantsToProne		: 1;
	uint8 Saved_bWantsToSlide		: 1;
	uint8 Saved_bWantsToDash		: 1;
	uint8 Saved_bWantsToWallRun		: 1;
	uint8 Saved_bWantsToHang		: 1;
	uint8 Saved_bWantsToClimb		: 1;
	uint8 Saved_bCustomJumpPressed	: 1;
	uint8 Saved_bWallRunIsRight		: 1;

	uint8 Saved_bHadAnimRootMotion	: 1;
	uint8 Saved_bTransitionFinished : 1;


	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
	virtual void Clear() override;
	virtual uint8 GetCompressedFlags() const override;
	virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;

	virtual void PrepMoveFor(ACharacter* C) override;
};
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
/*----------------------------------------------------*[Network Class]*---------------------------------------------------*/
class FNetworkPredictionData_Client_Synthetica : public FNetworkPredictionData_Client_Character
{

public:
	FNetworkPredictionData_Client_Synthetica(const UCharacterMovementComponent& ClientMovement);
	typedef FNetworkPredictionData_Client_Character Super;
	virtual FSavedMovePtr AllocateNewMove() override;
};


public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement (float DeltaSeconds) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	bool last = false;
public:
	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

	UFUNCTION(BlueprintPure) 
	bool IsMovementMode(EMovementMode InMovementMode) const;

	virtual bool IsMovingOnGround() const override;

	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;


private: /*Helper*/
	bool IsServer() const;

	float CapRadius() const;
	float CapHalfHeight() const;

private:
	bool Safe_bTransitionFinished;
	bool Safe_bHadAnimRootMotion;

	FString TransitionName;
	TSharedPtr<FRootMotionSource_MoveToForce>	TransitionRMS;
	UPROPERTY(Transient) UAnimMontage*			TransitionQueuedMontage;
	float										TransitionQueuedMontageSpeed;
	int											TransitionRMS_ID;

/*------------------------------------------------------------------------------------------------------------------------*/
																			/*R*/
				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma region Constructors
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*---------------------------------------------------*[Constructor]*------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
public:
	UBaseCharacterMovementComponent();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	virtual void InitializeComponent() override;
	UPROPERTY(Transient) 
	class ABaseCharacter* BaseCharacter;
	class UAnimInstance* AnimInstance;

#pragma endregion | Constructors |


#pragma region |Functions|
/*------------------------------------------------------------------------------------------------------------------------*/
																			/*R*/
				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*----------------------------------------------------*[Functions]*-------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma endregion|Functions|


#pragma region |General Variables|
/*------------------------------------------------------------------------------------------------------------------------*/

				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Movement|Variables]*---------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
public:
	UPROPERTY(BlueprintReadWrite, Category = Movement)
	float CurrentVelocitySize;

private:
	float CurrentMaxSpeed;
	UPROPERTY(EditDefaultsOnly)	float Walk_MaxWalkSpeed = 400.0f;
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma endregion|Variables|

#pragma region |Jump|
public:
	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;
#pragma endregion

#pragma region |Sprint|
/*------------------------------------------------------------------------------------------------------------------------*/
//																														  //
//																					/*R*/								  //
//					   /*H*/													/*F*/	/*E*/						      //
//				  /*T*/	   /*E*/											/*0*/			/*V*/					      //
//			 /*N*/			   /*T*/									/**/					/*E*/					  //
//		/*Y*/					   /*I*/							/*E*/							/*R*/				  //
//	/*S*/							   /*C*/					/*G*/									/*S*/			  //
//										   /*A*/			/*A*/											/*E*/		  //
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[Sprint]*--------------------------------------------------------*/
public:
	// Check if character is currently allowed to sprint.
	bool CanSprint();
	// Check if character is currently sprinting.
	bool IsSprinting();
	// Check if sprint input is active on the controller.
	bool IsControllerWantSprint() const;

protected:
	// Flag indicating character's intent to sprint.
	bool Safe_bWantsToSprint;
	// Flag indicating if character is currently sprinting.
	bool bIsSprinting;

private:
	// Maximum speed while sprinting.
	UPROPERTY(EditDefaultsOnly)
	float Sprint_MaxWalkSpeed = 800.0f;

public:
	// Set the sprint input trigger.
	// Called during character's input processing.
	void SetSprintInput(bool Trigger);

protected:
	// Start sprinting action.
	void StartSprinting();
	// Stop sprinting action.
	void StopSprinting();

public:
	// Getter function to determine whether the character wants to sprint or not.
	// Check if character intends to sprint.
	FORCEINLINE bool GetSprintButtonPressedOrNot() const { return Safe_bWantsToSprint; }
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma endregion|Sprint|


#pragma region |Crouch|
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[Crouch]*--------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
//																														  //
//																					/*R*/								  //
//					   /*H*/													/*F*/	/*E*/						      //
//				  /*T*/	   /*E*/											/*0*/			/*V*/					      //
//			 /*N*/			   /*T*/									/**/					/*E*/					  //
//		/*Y*/					   /*I*/							/*E*/							/*R*/				  //
//	/*S*/							   /*C*/					/*G*/									/*S*/			  //
//										   /*A*/			/*A*/											/*E*/		  //
/*------------------------------------------------------------------------------------------------------------------------*/
public:
	bool IsCrouchingCustom();
	bool CanCrouch();

private:
	bool bIsCrouching;

	// Code Chunks Locker for executions
	bool bEnterCrouchBlockExecuted = false;
	bool bExitCrouchBlockExecuted  = false;
private:
	bool Safe_bWantsToCrouch;

protected:
	void StartCrouch();
	void StopCrouch();

private:
	float Crouch_MaxWalkSpeed = 100.f;

public: 
	void SetCrouchInput(bool Trigger);


/*------------------------------------------------------------------------------------------------------------------------*/
#pragma endregion|Crouch|


#pragma region |Slide|
/*------------------------------------------------------------------------------------------------------------------------*/
//																														  //
//																					/*R*/								  //
//					   /*H*/													/*F*/	/*E*/						      //
//				  /*T*/	   /*E*/											/*0*/			/*V*/					      //
//			 /*N*/			   /*T*/									/**/					/*E*/					  //
//		/*Y*/					   /*I*/							/*E*/							/*R*/				  //
//	/*S*/							   /*C*/					/*G*/									/*S*/			  //
//										   /*A*/			/*A*/											/*E*/		  //
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[Slide]*---------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
protected:
	UPROPERTY(EditDefaultsOnly)	float Slide_MinSpeed			= 400.f;
	UPROPERTY(EditDefaultsOnly)	float Slide_MaxSpeed			= 400.f;
	UPROPERTY(EditDefaultsOnly)	float Slide_EnterImpulse		= 1000.f;
	UPROPERTY(EditDefaultsOnly)	float Slide_GravityForce		= 4000.f;
	UPROPERTY(EditDefaultsOnly)	float Slide_FrictionFactor		= 0.06f;
	UPROPERTY(EditDefaultsOnly) float Slide_BrakingDeceleration = 1000.f;

private:
	void EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);

	bool GetSlideSurface(FHitResult& Hit) const;
public:
	void SetSlideInput(bool Trigger);


	bool ControllerWantsToSlide();

	bool IsSliding();
	bool CanSlide();
	bool ShouldAllowSlideButtonPress();
private:
	bool bCanInitiateSlideCooldown = false;

	float CanPressSlideButtonTimer;
	float CanSlideAgainDelay = 4.0f;
	float CanCancelSlideButtonTimer;
	float CanCancelSlideButtonDelay;
private:
	bool Safe_bWantsToSlide;


private:
	UPROPERTY(EditDefaultsOnly)	float PlayerCanSlideAgainDelayTimer = 5.0f;

	bool bCanSlideAgain = true;
	bool bIsSliding;

public:
	FORCEINLINE bool GetIsSliding()			const { return bIsSliding; }
	FORCEINLINE bool GetSafeWantsToSlide()  const { return Safe_bWantsToSlide; }
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma endregion	|Slide|

#pragma region | Prone | 
/*------------------------------------------------------------------------------------------------------------------------*/
//																														  //
//																					/*R*/								  //
//					   /*H*/													/*F*/	/*E*/						      //
//				  /*T*/	   /*E*/											/*0*/			/*V*/					      //
//			 /*N*/			   /*T*/									/**/					/*E*/					  //
//		/*Y*/					   /*I*/							/*E*/							/*R*/				  //
//	/*S*/							   /*C*/					/*G*/									/*S*/			  //
//										   /*A*/			/*A*/											/*E*/		  //
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[Prone]*---------------------------------------------------------*/
protected:
	UPROPERTY(EditDefaultsOnly) float Prone_EnterHoldDuration	 = .2f;
	UPROPERTY(EditDefaultsOnly) float Prone_SlideEnterImpulse	 = 300.f;
	UPROPERTY(EditDefaultsOnly) float Prone_MaxSpeed			 = 150.f;
	UPROPERTY(EditDefaultsOnly) float Prone_BrakingDeceleration	 = 2500.f;

private:
	void EnterProne(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitProne();
	void PhysProne(float deltaTime, int32 Iterations);

public:
	bool CanProne() const;
	bool IsProne() const;
	void SetProneInput(bool Trigger);

	bool ControllerWantsToProne();

	bool IsProneButtonActive();
private:
	bool Safe_bWantsToProne;
	bool bIsProne;

	bool bProneInputCooldownActive = false;
	float ProneButtonTimer;
	float PRoneButtonDelay = 1.5f;

private: /*CrouchSys|Server|*/
	UFUNCTION(Server,Reliable)	void Server_EnterProne();
	

public: /*CrouchSys|Getter&Setters|*/
	FORCEINLINE bool GetIsProne() const { return bIsProne; }

	FORCEINLINE void SetProneInputCooldown(bool bActive);

/*------------------------------------------------------------------------------------------------------------------------*/
#pragma endregion

#pragma region |Dash|
public: // Deleegates
	UPROPERTY(BlueprintAssignable) FDashStartDelegate DashStartDelegate;

public:	// Properties
	UPROPERTY(EditDefaultsOnly)						 float DashImpulse			     = 1300.f;
	UPROPERTY(EditDefaultsOnly)						 float DashCooldownDuration		 = 1.f;
	UPROPERTY(EditDefaultsOnly)						 float AuthDashCooldownDuration  = .9f;

		// Montages
	UPROPERTY(EditDefaultsOnly, Category = Montages) 
	UAnimMontage* DashMontage;

public:	// Timer
	FTimerHandle DashCooldown_Timerhandle;
	float DashStartTime;

private: // Input
	bool Safe_bWantsToDash;

private: // Replication
	UPROPERTY(ReplicatedUsing = OnRep_DashStart) bool Proxy_bDashStart;
	UFUNCTION() void OnRep_DashStart();

public: // Functions
	bool CanDash() const;
	void PerformDash();

	void OnDashCooldownFinished();

	bool ControllerWantsToDash();

public: // Getter && Setters
	void SetDashInput(bool bTrigger);

#pragma endregion|Dash|
#pragma region |Mantle|

private: /* Properties */

	UPROPERTY(EditDefaultsOnly) float MantleMaxDistance				= 200.0f;
	UPROPERTY(EditDefaultsOnly) float MantleReachHeight				= 50.00f;
	UPROPERTY(EditDefaultsOnly) float MinMantleDepth				= 30.00f;
	UPROPERTY(EditDefaultsOnly) float MantleMinWallSteepnessAngle	= 75.00f;
	UPROPERTY(EditDefaultsOnly) float MantleMaxSurfaceAngle			= 40.00f;
	UPROPERTY(EditDefaultsOnly) float MantleMaxAlignmentAngle		= 45.00f;

	/* Montages */
	UPROPERTY(EditDefaultsOnly, Category = Montages)	UAnimMontage* TransitionShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)	UAnimMontage* TransitionTallMantleMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)	UAnimMontage* ProxyShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)	UAnimMontage* ProxyTallMantleMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)	UAnimMontage* ShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)	UAnimMontage* TallMantleMontage;

private: /* Replications */

	UPROPERTY(ReplicatedUsing = OnRep_ShortMantle) bool Proxy_bShortMantle;
	UPROPERTY(ReplicatedUsing = OnRep_TallMantle)  bool Proxy_bTallMantle;

	UFUNCTION() void OnRep_ShortMantle();
	UFUNCTION() void OnRep_TallMantle();

private:
	bool TryMantle();

	FVector GetMantleStartLocation(FHitResult FrontHit, FHitResult SurfaceHit, bool bTallMantle) const;


#pragma endregion

#pragma region |Wall-Run|
/*------------------------------------------------------------------------------------------------------------------------*/
//																														  //
//																					/*R*/								  //
//					   /*H*/													/*F*/	/*E*/						      //
//				  /*T*/	   /*E*/											/*0*/			/*V*/					      //
//			 /*N*/			   /*T*/									/**/					/*E*/					  //
//		/*Y*/					   /*I*/							/*E*/							/*R*/				  //
//	/*S*/							   /*C*/					/*G*/									/*S*/			  //
//										   /*A*/			/*A*/											/*E*/		  //
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*-----------------------------------------------------*[WallRun]*--------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/

private:
	bool Safe_bWallRunIsRight;

private:
	UPROPERTY(EditDefaultsOnly) float WallRun_MinWalkSpeed		= 300.0f;
	UPROPERTY(EditDefaultsOnly) float WallRun_MaxWalkSpeed		= 800.0f;
	UPROPERTY(EditDefaultsOnly) float MaxVerticalWallRunSpeed	= 300.0f;
	UPROPERTY(EditDefaultsOnly) float WallRunPullAwayAngle		= 75.0f;
	UPROPERTY(EditDefaultsOnly) float WallAttractionForce		= 200.0f;
	UPROPERTY(EditDefaultsOnly) float MinWallRunHeight			= 50.0f;
	UPROPERTY(EditDefaultsOnly) float WallJumpOffForce			= 450.0f;
	UPROPERTY(EditDefaultsOnly) UCurveFloat* WallRunGravityScaleCurve;

private:
	bool TryWallRun();
	void PhysWallRun(float DeltaTime, int32 Iterations);

public:
	bool IsWallRunning() const { return IsCustomMovementMode(CMOVE_WallRun); }
	bool WallRunningIsRight() { return Safe_bWallRunIsRight; }
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma endregion|Wall-Run|

#pragma region|Hang|

#pragma endregion|Hang|

#pragma region |Climb|
#pragma endregion|Climb|



#pragma region |Getters| && |Setters|
/*------------------------------------------------------------------------------------------------------------------------*/

				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*-----------------------------------------------------*[Getters]*--------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentVelocitySize() const { return CurrentVelocitySize; }
/*------------------------------------------------------------------------------------------------------------------------*/

				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*-----------------------------------------------------*[Setters]*--------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/

#pragma endregion|Getters| && |Setters|









#pragma region Development Place

/*------------------------------------------------------------------------------------------------------------------------*/

				/*H*/													/*F*/	/*E*/
			/*T*/	/*E*/											/*0*/			/*V*/
		/*N*/			/*T*/									/**/					/*E*/
	/*Y*/					/*I*/							/*E*/							/*R*/
/*S*/							/*C*/					/*G*/									/*S*/
									/*A*/			/*A*/											/*E*/
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*-----------------------------------------------------*[DevTest]*--------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma endregion | Reserved for new integrations, first, test in here. |
#pragma endregion


#pragma region DEBUG TOOLS
private:
	void DebugLog_CustomAndDefaultMovementState();

};
#pragma endregion
