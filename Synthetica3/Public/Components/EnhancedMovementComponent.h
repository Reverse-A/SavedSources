// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

#include "EnhancedMovementComponent.generated.h"


#pragma region Custom Movement Mode - ENUM
UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Crouch		UMETA(DisplayName = "Crouch"),
	CMOVE_Prone			UMETA(DisplayName = "Prone"),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_Dash			UMETA(DisplayName = "Dash"),
	CMOVE_WallRun		UMETA(DisplayName = "Wall Run"),
	CMOVE_Hang			UMETA(DisplayName = "Hang"),
	CMOVE_Climb			UMETA(DisplayName = "Climb"),
	CMOVE_MAX			UMETA(Hidden),
};
#pragma endregion

#pragma region | Class |
UCLASS()
class SYNTHETICA_API UEnhancedMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Synthetica : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		public:
		enum CompressedFlags
		{
			FLAG_Crouch		= 0x1,
			FLAG_Slide		= 0x2,
			FLAG_Prone		= 0x4,
			FLAG_Dash		= 0x8,
			FLAG_WallRun	= 0x10,
			FLAG_Hang		= 0x20,
			FLAG_Climb		= 0x40,
			FLAG_Sprint     = 0x80,
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

		FSavedMove_Synthetica();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Synthetica : public FNetworkPredictionData_Client_Character
	{
		public:
			FNetworkPredictionData_Client_Synthetica(const UCharacterMovementComponent& ClientMovement);

			typedef FNetworkPredictionData_Client_Character Super;

			virtual FSavedMovePtr AllocateNewMove() override;
	};


public:
	class ABaseCharacter* BaseCharacter;

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

public:
	UFUNCTION(BlueprintPure) 
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

	UFUNCTION(BlueprintPure) 
	bool IsMovementMode(EMovementMode InMovementMode) const;


public:
	UEnhancedMovementComponent();
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void InitializeComponent() override;

public:
	virtual bool							IsMovingOnGround()				const override;
	virtual bool							CanCrouchInCurrentState()		const override;
	virtual float							GetMaxSpeed()					const override;
	virtual float							GetMaxBrakingDeceleration()		const override;



	// Character's angles and similiar action functions and variables
	void CalculateSlopeAngle(float DeltaTime);

	float GetSlopePitchDegreeAngle() const;
	float GetSlopRollDegreeAngle() const;

	UPROPERTY(Replicated)
	float OutSlopePitchDegreeAngle;
	float OutSlopeRollDegreeAngle;

	float SlopeDegreeResetTimer;
	bool bSlopeHitInRange;
	bool InSlopeMovement;
	bool bVeryHighSlope;
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*-----------------------------------------------------*[Server]*---------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
FNetBitWriter Synthetica_ServerMoveBitWriter;

protected:
	virtual void OnClientCorrectionReceived(class FNetworkPredictionData_Client_Character& ClientData, float TimeStamp, FVector NewLocation, FVector NewVelocity, UPrimitiveComponent* NewBase, FName NewBaseBoneName, bool bHasBase, bool bBaseRelativePosition, uint8 ServerMovementMode, FVector ServerGravityDirection) override;

private:
int TickCount = 0;
int CorrectionCount = 0;
int TotalBitsSent = 0;
float AccumulatedClientLocationError = 0.f;


/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*---------------------------------------------------*[Transitions]*------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
private:
bool Safe_bTransitionFinished;

TSharedPtr<FRootMotionSource_MoveToForce> TransitionRMS;

FString TransitionName;

UPROPERTY(Transient)
UAnimMontage* TransitionQueuedMontage;

float TransitionQueuedMontageSpeed;
int TransitionRMS_ID;

bool Safe_bHadAnimRootMotion;



float StoredDeltaTime;
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*----------------------------------------------------*[Helpers]*---------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
private:
	float CapRadius()		const;
	float CapHalfHeight()	const;
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*-----------------------------------------------------*[Bases]*----------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
private:
	float CurrentMaxSpeed;
	float CurrentVelocitySize;
	UPROPERTY(EditDefaultsOnly)
	float Sprint_MaxWalkSpeed	= 800.0f;

	UPROPERTY(EditDefaultsOnly)
	float Jog_MaxWalkSpeed		= 400.0f;

	UPROPERTY(EditDefaultsOnly)
	float Crouch_MaxWalkSpeed	= 175.0f;

	UPROPERTY(EditDefaultsOnly)
	float Prone_MaxWalkSpeed	= 75.00f;

	UPROPERTY(EditDefaultsOnly)
	float Aim_MaxWalkSpeed		= 175.0f;

public:
	FORCEINLINE float GetVelocitySize() const { return CurrentVelocitySize; }

public:
	void AutoRun();
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[Jump]*----------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma region |Jump|
public:
	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;

#pragma endregion
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[Sprint]*--------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma region |Sprint|
public:
	// Check if Character is currently allowed to sprint
	bool CanSprint();
	// Check if character is currently sprinting.
	bool IsSprinting();
	// Check if sprint input is active on the controller.
	bool IsControllerWantToSprint() const;

protected:
	void IncreaseMovementSpeed_SprintMaxSpeed();
	void DecreaseMovementSpeed_AfterSprint();



public:
	// Set the sprint input trigger. Called during character's input processing.
	void SetSprintInput(bool Trigger);

protected:
	// Flag indicating character's intent to sprint.
	bool Safe_bWantsToSprint;

	// Flag indicating if character is currently sprinting.
public:
	FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }


////////* Server */////////////
protected:
	// A replicated boolean property to track whether the entity is currently sprinting
	UPROPERTY(Replicated) 
	bool bIsSprinting;

	UFUNCTION(Server, Reliable)
	void Server_ActiveSprint();

	UFUNCTION(Server, Reliable)
	void Server_DeactiveSprint();
#pragma endregion 
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[Prone]*---------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma region |Prone|
public:
	// Check if the entity can go into a prone position (lying down)
	bool CanProne();
	// Check if the entity is currently in the prone position
	bool IsProning();
	// Check if sprint input is active on the controller.
	bool IsControllerWantsToProne() const;

	void SetProneInput(bool Trigger);

private:
	UFUNCTION(Server, Reliable) 
	void Server_EnterProne();
	UFUNCTION(Server, Reliable)
	void Server_ExitProne();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EnterProne();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ExitProne();

	// Enter the prone position, called when transitioning from another movement mode
	void EnterProne(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	// Exit the prone position, called when leaving the prone state
	void ExitProne();
	// Perform physical simulation for the prone position, typically called each frame
	void PhysProne(float deltaTime, int32 Iterations);

	void OnTryEnterProne() { Safe_bWantsToProne = true; }

protected:
	UPROPERTY(Replicated) bool bIsProning;

	bool Safe_bWantsToProne = false;
private:
	UPROPERTY(EditDefaultsOnly, meta = (PrivateAccess = "true")) float Prone_EnterHoldDuration		= 0.2000f;
	UPROPERTY(EditDefaultsOnly, meta = (PrivateAccess = "true")) float Prone_SlideEnterImpulse		= 300.00f;
	UPROPERTY(EditDefaultsOnly, meta = (PrivateAccess = "true")) float Prone_MaxSpeed				= 300.00f;
	UPROPERTY(EditDefaultsOnly, meta = (PrivateAccess = "true")) float Prone_BrakingDeceleration	= 2500.0f;

public:
	FORCEINLINE bool GetIsProning() const { return bIsProning; }
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*-----------------------------------------------------*[Crouch]*---------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
public:
	bool CanCrouch();
	bool IsCrouching_Custom();

private:
	bool bIsCrouched;
#pragma endregion 
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[Slide]*---------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma region |Slide|
public:
	bool IsControllerWantsSlide();

	// Check if the entity can perform a sliding action
	bool CanSlide();
	// Check if the entity is currently in a sliding state
	bool IsSliding();


	// Enter the sliding state, called when transitioning from another movement mode
	void EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	UFUNCTION(Server, Reliable)
	void Server_EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);

	// Exit the sliding state, called when leaving the sliding state
	void ExitSlide();
	// Perform physical simulation for the sliding state, typically called each frame
	void PhysSlide(float deltaTime, int32 Iterations);


private:
	UPROPERTY(EditDefaultsOnly) float Slide_MinSpeed					= 400.0f;
	UPROPERTY(EditDefaultsOnly) float Slide_MaxSpeed					= 400.0f;
	UPROPERTY(EditDefaultsOnly) float Slide_EnterImpulse				= 750.0f;
	UPROPERTY(EditDefaultsOnly) float Slide_GravityForce				= 2000.f;
	UPROPERTY(EditDefaultsOnly) float Slide_FrictionFactor				= 0.060f;
	UPROPERTY(EditDefaultsOnly) float Slide_BrakingDeceleration			= 1000.f;

private: 
	bool bEnterSlideExecution;
	bool bExitSlideExecution;

	class UButton* TrainingButton;
	
	bool bCanInitiateSlideCooldown = false;
	float CanPressSlideButtonTimer;
	float CanSlideAgainDelay = 4.0f;
	float CanCancelSlideButtonTimer;
	float CanCancelSlideButtonDelay;
protected:
	// Flag indicating character's intent to slide.
	bool bWantsToSlide;
	// Flag indicating if character is currently sliding.
	UPROPERTY(Replicated)
	bool bIsSliding;

public:
	void SetSlideInput(bool Trigger);

	FORCEINLINE bool GetIsSliding() const { return bIsSliding; }

#pragma endregion 
/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[Slide]*---------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
private:
	bool Safe_bWallRunIsRight;

private:
	UPROPERTY(EditDefaultsOnly) float WallRun_MinWalkSpeed = 300.0f;
	UPROPERTY(EditDefaultsOnly) float WallRun_MaxWalkSpeed = 800.0f;
	UPROPERTY(EditDefaultsOnly) float MaxVerticalWallRunSpeed = 300.0f;
	UPROPERTY(EditDefaultsOnly) float WallRunPullAwayAngle = 75.0f;
	UPROPERTY(EditDefaultsOnly) float WallAttractionForce = 200.0f;
	UPROPERTY(EditDefaultsOnly) float MinWallRunHeight = 50.0f;
	UPROPERTY(EditDefaultsOnly) float WallJumpOffForce = 450.0f;
	UPROPERTY(EditDefaultsOnly) UCurveFloat* WallRunGravityScaleCurve;

private:
	bool TryWallRun();
	void PhysWallRun(float DeltaTime, int32 Iterations);

	bool bIsWallRunning;

	float GravityThresholdDelay = 0.5f;
	float GravityResetTimer;

	float MaxWallRunTime = 2.5f;
	float MaxWallRunDelay;

public:
	bool IsWallRunning() const { return IsCustomMovementMode(CMOVE_WallRun); }
	bool WallRunningIsRight() { return Safe_bWallRunIsRight; }

	FORCEINLINE bool GetIsWallRunning() const {	return bIsWallRunning; }


/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*-------------------------------------------*[Climb And Hang And Mantle]*------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/

private:
UPROPERTY(EditDefaultsOnly) UAnimMontage* TransitionHangMontage;
UPROPERTY(EditDefaultsOnly) UAnimMontage* WallJumpMontage;
UPROPERTY(EditDefaultsOnly) float WallJumpForce = 400.f;

UPROPERTY(EditDefaultsOnly) float Climb_MaxSpeed = 300.f;
UPROPERTY(EditDefaultsOnly) float Climb_BrakingDeceleration = 1000.f;
UPROPERTY(EditDefaultsOnly) float Climb_ReachDistance = 200.f;


// Mantle
UPROPERTY(EditDefaultsOnly) float MantleMaxDistance				= 200.0f;
UPROPERTY(EditDefaultsOnly) float MantleReachHeight				= 50.00f;
UPROPERTY(EditDefaultsOnly) float MinMantleDepth				= 30.00f;
UPROPERTY(EditDefaultsOnly) float MantleMinWallSteepnessAngle	= 75.00f;
UPROPERTY(EditDefaultsOnly) float MantleMaxSurfaceAngle			= 40.00f;
UPROPERTY(EditDefaultsOnly) float MantleMaxAlignmentAngle		= 45.00f;
UPROPERTY(EditDefaultsOnly) UAnimMontage* TallMantleMontage;
UPROPERTY(EditDefaultsOnly) UAnimMontage* ShortMantleMontage;
UPROPERTY(EditDefaultsOnly) UAnimMontage* TransitionTallMantleMontage;
UPROPERTY(EditDefaultsOnly) UAnimMontage* TransitionShortMantleMontage;
UPROPERTY(EditDefaultsOnly) UAnimMontage* ProxyTallMantleMontage;
UPROPERTY(EditDefaultsOnly) UAnimMontage* ProxyShortMantleMontage;

//bool TryHang();
//bool TryClimb();
bool TryMantle();

//void PhysClimb(float deltaTime, int32 Iterations);


FVector GetMantleStartLocation(FHitResult FrontHit, FHitResult SurfaceHit, bool bIsTallMantle) const;

FVector Mantle_TargetLocation;
FVector Mantle_UpTransitionTarget;

UPROPERTY(Replicated)
float MantleTransitionTime = 0.0f;
UPROPERTY(Replicated)
float MantleTransitionDuration = 1.0f;

UPROPERTY(Replicated)
bool bTallMantle;

FVector ShortMantleTarget;
FVector TallMantleTarget;


bool bCanMantle;
bool bIsMantle;
bool bMantleMovementModeGate = true;
void SetMantleLocation();

UFUNCTION(NetMulticast, Reliable)
void Multicast_SetTallMantleLocation();

bool Mantle_FirstThreshold = true;
bool Mantle_SecondThreshold = false;
public:
UFUNCTION(BlueprintPure) bool IsHanging() const { return IsCustomMovementMode(CMOVE_Hang); }
UFUNCTION(BlueprintPure) bool IsClimbing() const { return IsCustomMovementMode(CMOVE_Climb); }

FORCEINLINE bool GetCanMantle() const { return bCanMantle; }
FORCEINLINE bool GetTallMantle() const { return bTallMantle; }
FORCEINLINE bool IsMantling() const { return bIsMantle; }


private:
UPROPERTY(ReplicatedUsing = OnRep_ShortMantle) bool Proxy_bShortMantle;
UPROPERTY(ReplicatedUsing = OnRep_TallMantle) bool Proxy_bTallMantle;

UFUNCTION() void OnRep_ShortMantle();
UFUNCTION() void OnRep_TallMantle();






void DebugEachClient();
};
#pragma endregion | Class |