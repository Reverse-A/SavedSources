/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <WeaponMastery/Public/Component/WeaponMasteryComponent.h>
#include "WeaponModule.generated.h"

class ACharacterModule;

class USphereComponent;
class UStaticMeshComponent;
class USoundCue;

class UWeaponMasteryComponent;

UCLASS(Blueprintable)
class WEAPONMASTERY_API AWeaponModule : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponModule();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#pragma region OverlapEvents
protected:
	// Handles collision overlap event
	UFUNCTION()
	void OnCollisionOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// Handles collision end overlap event
	UFUNCTION()
	void OnCollisionEndOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void OnWeaponHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
#pragma endregion
#pragma region Classes&Componens
protected:
	// Reference to the character using the WeaponMastery
	UPROPERTY()
	TObjectPtr<ACharacterModule> OwningCharacter;

	// Collision component to detect overlap events
	UPROPERTY(VisibleAnywhere, Category = "ArsenalSystem|Fundamentals")
	TObjectPtr<USphereComponent> AreaCollision;

	// Static mesh component to represent the weapon visually
	UPROPERTY(VisibleAnywhere, Category = "ArsenalSystem|Fundamentals")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon Mastery Component", meta = (SortPriority = "1"))
	TObjectPtr<UWeaponMasteryComponent> WeaponMasteryComponent;

// Pre-prepared getters for future use of class variables in other projects
public:
	// Returns the collision component
	TObjectPtr<USphereComponent> GetAreaCollision() const
	{
		return AreaCollision;
	}

	// Returns the weapon mesh component
	TObjectPtr<UStaticMeshComponent> GetWeaponMesh() const
	{
		return WeaponMesh;
	}

	TObjectPtr<UWeaponMasteryComponent> GetWeaponMasteryComponent()
	{
		return WeaponMasteryComponent;
	}
#pragma endregion
#pragma region Weapon-State
public:
	void SetWeaponState(EWeaponState NewState);
	void SwitchWeaponState(EWeaponState InWeaponState);

protected:
	void HandleWeaponStates();
	void HandLeInitialState();
	void HandleEquipState();
	void HandleEquipSecondaryState();
	void HandleDropState();
	void HandleTradeState();
	void HandlePurchaseState();
	void HandleLootState();
	void HandleCraftState();
	void HandleUpgradeState();
	void HandleRewardState();
	void HandleBrokenState();
	void HandleInventoryState();
	void HandleStorageState();
#pragma endregion
#pragma region VisualFxs

#pragma endregion
#pragma region SoundFxs
private:
	// Plays selected sound
	UFUNCTION(BlueprintCallable, Category = "Functions|Sound")
	void Play2DSound(USoundCue * InSoundCue);
#pragma endregion
};
