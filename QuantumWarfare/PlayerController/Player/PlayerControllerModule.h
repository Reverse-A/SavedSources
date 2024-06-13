// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "PlayerControllerModule.generated.h"

class APlayerCharacter;

class UAbilitySystem;
class UCharacterMovementComponent;
class UDamageTextComponent;
class UInputMappingContext;
class UInputAction;
class UPlayerInputConfig;
class USplineComponent;

class IEnemyInterface;

struct FInputActionValue;


UCLASS()
class QUANTUMWARFARE_API APlayerControllerModule : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerControllerModule();

	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

#pragma region Classes and Components
private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
#pragma endregion
#pragma region MappingContext and Input Actions
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> HoldPlayerAction;

#pragma endregion

#pragma region InputFunctions
private:
	void Move(const FInputActionValue& InputActionValue);

	void Sprint();
	void Sprint_Completed();

	void Interaction();

	void Jump();

	bool bHoldPlayerKeyDown = false;
	void HoldPlayerKeyPressed() { bHoldPlayerKeyDown = true; };
	void HoldPlayerKeyReleased() { bHoldPlayerKeyDown = false; };
#pragma endregion

#pragma region CursorTracers
private:
	void CursorTrace();

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	FHitResult CursorHit;
#pragma endregion
#pragma region InputTag
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UPlayerInputConfig> InputConfig;

#pragma endregion

	UPROPERTY()
	TObjectPtr<UAbilitySystem> AbilitySystem;

	UAbilitySystem* GetAbilitySystem();

	FVector CachedDestination	  = FVector::ZeroVector;
	float FollowTime			  = 0.0f;
	float ShortPressThreshold	  = 0.5f;
	bool bAutoRunning			  = false;
	bool bTargeting				  = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.0f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
public:
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockHit, bool bCriticalHit);
};
