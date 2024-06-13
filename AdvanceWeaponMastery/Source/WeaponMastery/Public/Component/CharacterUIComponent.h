/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterUIComponent.generated.h"

class ACharacterModule;

class UUserWidget;
class AWeaponModule;
class UShortWeaponInfoWidget;
class URangeWeaponInfoWidget;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONMASTERY_API UCharacterUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterUIComponent();

	// Function to set the character associated with this character ui component.
	void InitializeCharacterReference(ACharacterModule* InCharacter);
protected:
	virtual void BeginPlay() override;

private:
	// Pointer to the character that owns this component.
	UPROPERTY()
	TObjectPtr<ACharacterModule> OwningCharacter;

public:
	void CreateWeaponInformationWidgetAddToScreen();
	void UpdateShortRangeWeaponInformationWidget(AWeaponModule* InWeaponModule);
	void UpdateLongRangeWeaponInformationWidget(AWeaponModule* InWeaponModule);
	void HideVisibleWeaponInformationWidgets();
	
private:
	// The widget that will enable us to publish all the information of the melee weapon's properties.
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> MainOverlay;

	// The widget that will enable us to publish all the information of the melee weapon's properties.
	UPROPERTY(EditAnywhere, Category = "Widget|WeaponInformation")
	TSubclassOf<UUserWidget> ShortWeaponInformationWidget;

	// The widget that will enable us to publish all the information of the melee weapon's properties.
	UPROPERTY(EditAnywhere, Category = "Widget|WeaponInformation")
	TSubclassOf<UUserWidget> LongRangeWeaponInformationWidget;

	// Short Range Type Weapon Information WidgetClass
	UShortWeaponInfoWidget* ShortRangeWeaponInformationWidgetInstance;

	// Long Range Type Weapon Information WidgetClass
	URangeWeaponInfoWidget* LongRangeWeaponInformationWidgetInstance;
};
