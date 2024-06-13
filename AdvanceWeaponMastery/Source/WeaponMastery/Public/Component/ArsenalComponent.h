/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArsenalComponent.generated.h"

class ACharacterModule;
class AWeaponModule;
class AShortRangeWeaponModule;
class ALongRangeWeaponModule;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONMASTERY_API UArsenalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArsenalComponent();

	// Function to set the character associated with this arsenal system.
	void InitializeCharacterReference(ACharacterModule* InCharacter);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:	
	// Pointer to the character that owns this component.
	UPROPERTY()
	TObjectPtr<ACharacterModule> OwningCharacter;

	// Pointer to the weapon that is currently being overlapped (in range to be picked up).
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<AWeaponModule> PendingPickupWeapon;

	// Pointer to the weapon that is currently equipped by the character.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<AWeaponModule> CurrentlyEquippedWeapon;

public:
	// Declares a function named UpdatePendingPickupWeapon.
	// This function is responsible for updating the pending pickup weapon with the specified weapon module.
	// The pending pickup weapon is the weapon the player is currently interacting with and may potentially equip.
	// The function takes a pointer to a weapon module (InOverlapWeapon) as input.
	void UpdatePendingPickupWeapon(AWeaponModule* InOverlapWeapon);

	// Getter for the currently overlapped weapon.
	AWeaponModule* GetPendingPickupWeapon() const
	{
		return PendingPickupWeapon;
	}
	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// These variables at below are designed to provide accessible references to weapon modules within the							//
// UArsenalSystemComponent class, enabling interaction with the class's functions, variables, and other data.					//
//																																//
// The MeleeWeapon and RangeWeapon pointers hold single instances of their respective weapon types,								//
// allowing for direct manipulation of the currently equipped weapons.															//
//																																//
// The MeleeWeapons and RangeWeapons arrays serve as preliminary setups for more complex inventory systems,						//
// where multiple weapons can be managed, swapped, and utilized.																//
//																																//
// By marking these variables with UPROPERTY specifiers such as BlueprintReadWrite, they become accessible from Blueprints,		//
// providing a bridge between C++ code and Unreal Engine's visual scripting system.												//
//																																//
// This setup facilitates seamless integration and interaction, preparing the groundwork for advanced gameplay mechanics		//
// such as dynamic weapon management and inventory systems in future development stages.										//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:
	// Holds a reference to a single melee weapon module
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<AShortRangeWeaponModule> ShortRangeWeapon;

	// Holds a reference to a single ranged weapon module
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<ALongRangeWeaponModule> LongRangeWeapon;

	// Array to hold multiple short range weapon modules
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TObjectPtr<AShortRangeWeaponModule>> ShortRangeWeapons;

	// Array to hold multiple long range weapon modules
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TObjectPtr<ALongRangeWeaponModule>> LongRangeWeapons;


	void ConvertShortRangeWeapon(AWeaponModule* InWeaponModule);
	void ConvertLongRangeWeapon(AWeaponModule* InWeaponModule);

public:
	// Function to equip a weapon, callable from Blueprints.
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipPendingPickupWeapon();

	// Declares a function named UnequipAndDropWeapon.
	// This function is responsible for unequipping and dropping the weapon held by the character.
	void UnequipAndDropWeapon();

	// Function to throw the equipped weapon, callable from Blueprints.
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ExecuteWeaponThrow();
};
