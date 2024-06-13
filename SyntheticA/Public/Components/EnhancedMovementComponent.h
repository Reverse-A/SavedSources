// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedMovementComponent.generated.h"

class ABaseCharacter;

#pragma region Custom Movement Mode - Types
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

	class FSavedMove_Custom : public FSavedMove_Character
	{
	public:
		enum CompressedFlags
		{
			FLAG_Sprint = 0x10,
			FLAG_Dash	= 0x20,
			FLAG_Custom_2 = 0x40,
			FLAG_Custom_3 = 0x80,
		};

		// Flags
		uint8 Saved_bWantsToSprint : 1;

		FSavedMove_Custom();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Custom : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Custom(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};
protected:
	UEnhancedMovementComponent();

public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	UPROPERTY()	ABaseCharacter* BaseCharacter;

protected:
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;


/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*-------------------------------------------------*[Velocity Values]*----------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
private:
	float CurrentMaxSpeed;
	float CurrentVelocitySize;

	UPROPERTY(EditDefaultsOnly)
	float Sprint_MaxThresholdSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly)
	float Sprint_MaxWalkSpeed = 800.0f;

	UPROPERTY(EditDefaultsOnly)
	float Jog_MaxWalkSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly)
	float Crouch_MaxWalkSpeed = 175.0f;

	UPROPERTY(EditDefaultsOnly)
	float Prone_MaxWalkSpeed = 75.00f;

	UPROPERTY(EditDefaultsOnly)
	float Aim_MaxWalkSpeed = 175.0f;

public:
	FORCEINLINE float GetVelocitySize() const { return CurrentVelocitySize; }

/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[ Jump ]*--------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
private:

/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[Sprint]*--------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma region |Sprint|
private:
	bool Safe_bWantsToSprint;

	UPROPERTY(Replicated)
	bool bIsSprinting;

	bool bSwitchSprint;

public:
	// Check if Character is currently allowed to sprint
	bool CanSprint();
	// Check if character is currently sprinting.
	bool IsSprinting();
	// Check if sprint input is active on the controller.
	bool IsControllerWantToSprint() const;

	void SprintButtonPressed();
	void SprintButtonReleased();

protected:
	void IncreaseSprintSpeed();
	void DecreaseSprintSpeed();
public:
	void SetSprintInput(bool SetValue);
public:
	FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }

	FORCEINLINE void SwitchSprint() { bSwitchSprint = !bSwitchSprint; }
#pragma endregion 

/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------\\------------------------\\*Synethica*//------------------------//----------------------------*/
/*------------------------------------------------*[Controller|Inputs]*---------------------------------------------------*/
/*------------------------------------------------------*[Crouch]*--------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
#pragma region |Crouch|
public:
	bool CanCrouch();
	void SetCrouchInput(bool SetValue);
#pragma endregion
};
#pragma endregion