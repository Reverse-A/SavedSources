/*
* Copyright (c) Reverse-A @2024
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterModule.generated.h"

#pragma region Forward Declaration
// Components
class UArsenalComponent;
class UCameraComponent;
class UCharacterUIComponent;
class UInputComponent;
class USkeletalMeshComponent;

// Actor classes
class AWeaponModule;
#pragma endregion


UCLASS()
class WEAPONMASTERY_API ACharacterModule : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ACharacterModule();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PreInitializeComponents() override;

private:
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	// Getter function for FirstPersonCameraComponent
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	// Arsenal component responsible for managing the character's weapons
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ArsenalSystem")
	TObjectPtr<UArsenalComponent> ArsenalComponent;

public:
	// Getter function for ArsenalComponent
	UArsenalComponent* GetArsenalComponent() { return ArsenalComponent; }

protected:
	// UI component responsible for managing the character's UI elements
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UIComponent")
	TObjectPtr<UCharacterUIComponent> CharacterUIComponent;

public:
	// Getter function for CharacterUIComponent
	TObjectPtr<UCharacterUIComponent> GetCharacterUIComponent() { return CharacterUIComponent; }

protected:
	// Function for performing raycast to gather object data, especially weapons and weapon properties
	void PerformRayCast();

private:
	// Flag indicating whether a weapon is equipped or not
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "CharacterActions")
	bool bWeaponEquipped = false;

public:
	// Function to set the weapon equipped flag
	void SetWeaponEquipped(bool InEquipped) { bWeaponEquipped = InEquipped; }
};
