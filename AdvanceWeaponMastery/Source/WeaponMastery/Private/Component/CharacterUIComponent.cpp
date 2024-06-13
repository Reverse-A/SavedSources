#include "Component/CharacterUIComponent.h"
#include "Components/TextBlock.h"

#include <WeaponMastery/Public/Widget/ShortWeaponInfoWidget.h>
#include <WeaponMastery/Public/Widget/RangeWeaponInfoWidget.h>

#include <WeaponMastery/Public/Actor/WeaponModule.h>

UCharacterUIComponent::UCharacterUIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UCharacterUIComponent::InitializeCharacterReference(ACharacterModule* InCharacter)
{
	OwningCharacter = InCharacter;
}

void UCharacterUIComponent::BeginPlay()
{
	Super::BeginPlay();
	CreateWeaponInformationWidgetAddToScreen();
}


void UCharacterUIComponent::CreateWeaponInformationWidgetAddToScreen()
{
	// This function creates and configures UI widgets for displaying weapon information. 
	// It first checks if the widget classes (WeaponInformationWidget and RangeWeaponInformationWidget) are valid. 
	// If they are, it creates instances of these widgets. 
	// Each widget is then added to the viewport, positioned at specific screen coordinates (X: 1000.0, Y: 500.0), 
	// and initially set to be hidden.
	// This setup allows the game to control when and where these widgets are displayed to the player,
	// ensuring they are only shown when needed and enhancing the player's experience by providing relevant weapon information
	// dynamically during gameplay.

	// Define the position where the widget will appear on the screen.
	FVector2D ViewportPosition;
	ViewportPosition.X = 1000.0f;	// X coordinate on the viewport.
	ViewportPosition.Y = 500.0f;	// Y coordinate on the viewport

	// Check if the ShortWeaponInformationWidget class is valid (not null).
	if (ShortWeaponInformationWidget != nullptr)
	{
		// Create an instance of the ShortWeaponInformationWidget and assign it to ShortRangeWeaponInformationWidgetInstance.
		ShortRangeWeaponInformationWidgetInstance = CreateWidget<UShortWeaponInfoWidget>(GetWorld(), ShortWeaponInformationWidget);

		// Check if the widget instance was successfully created.
		if (ShortRangeWeaponInformationWidgetInstance != nullptr)
		{
			// Add the widget to the viewport so it will be visible on the screen.
			ShortRangeWeaponInformationWidgetInstance->AddToViewport();

			// Set the initial position of the widget in the viewport.
			ShortRangeWeaponInformationWidgetInstance->SetPositionInViewport(ViewportPosition);

			// Initially hide the widget. It will be shown later based on game logic.
			ShortRangeWeaponInformationWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// Check if the RangeWeaponInformationWidget class is valid (not null).
	if (LongRangeWeaponInformationWidget != nullptr)
	{
		// Create an instance of the RangeWeaponInformationWidget and assign it to RangeWeaponInformationWidgetInstance.
		LongRangeWeaponInformationWidgetInstance = CreateWidget<URangeWeaponInfoWidget>(GetWorld(), LongRangeWeaponInformationWidget);

		// Check if the widget instance was successfully created.
		if (LongRangeWeaponInformationWidgetInstance != nullptr)
		{
			// Add the widget to the viewport so it will be visible on the screen.
			LongRangeWeaponInformationWidgetInstance->AddToViewport();

			// Set the initial position of the widget in the viewport.
			LongRangeWeaponInformationWidgetInstance->SetPositionInViewport(ViewportPosition);

			// Initially hide the widget. It will be shown later base on game logic.
			LongRangeWeaponInformationWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	

	// Check if MainOverlay is valid
	if (MainOverlay != nullptr)
	{
		// Create the widget
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MainOverlay);

		// Check if the widget instance is valid
		if (WidgetInstance != nullptr)
		{
			// Add the widget to the viewport
			WidgetInstance->AddToViewport();
		}
	}
}

void UCharacterUIComponent::UpdateShortRangeWeaponInformationWidget(AWeaponModule* InWeaponModule)
{
	if (InWeaponModule != nullptr)
	{
		// Check if the ShortRangeWeaponInformationWidgetInstance is valid (not null).
		if (ShortRangeWeaponInformationWidgetInstance != nullptr)
		{
			/* Update the text fields of the widget with the retrieved properties of the short range weapon.*/

			// Weapon Name
			FString ShortRangeWeaponName = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Name;
			ShortRangeWeaponInformationWidgetInstance->Name->SetText
			(
				FText::FromString(ShortRangeWeaponName)
			);

			// Weapon Rarity
			FString ShortRangeRarityName = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Rarity;
			ShortRangeWeaponInformationWidgetInstance->Rarity->SetText
			(
				FText::FromString(ShortRangeRarityName)
			);

			// Weapon Damage
			FString ShortRangeWeaponDamage = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Damage;
			ShortRangeWeaponInformationWidgetInstance->Damage->SetText
			(
				FText::FromString(ShortRangeWeaponDamage)

			);

			// Weapon Penetration
			FString ShortRangeWeaponPenetration = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Penetration;
			ShortRangeWeaponInformationWidgetInstance->Penetration->SetText
			(
				FText::FromString(ShortRangeWeaponPenetration)
			);

			// Weapon Range
			FString ShortRangeWeaponRange = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Range;
			ShortRangeWeaponInformationWidgetInstance->Range->SetText
			(
				FText::FromString(ShortRangeWeaponRange)
			);


			// Weapon Weight
			FString ShortRangeWeaponWeight = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Weight;
			ShortRangeWeaponInformationWidgetInstance->Weight->SetText
			(
				FText::FromString(ShortRangeWeaponWeight)
			);

			// Weapon AttackSpeed
			FString ShortRangeWeaponAttackSpeed = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().AttackSpeed;
			ShortRangeWeaponInformationWidgetInstance->AttackSpeed->SetText
			(
				FText::FromString(ShortRangeWeaponAttackSpeed)
			);

			// Weapon Durability
			FString ShortRangeWeaponDurability = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Durability;
			ShortRangeWeaponInformationWidgetInstance->Durability->SetText
			(
				FText::FromString(ShortRangeWeaponDurability)
			);

			// Weapon Price
			FString ShortRangeWeaponPrice = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Price;
			ShortRangeWeaponInformationWidgetInstance->Price->SetText
			(
				FText::FromString(ShortRangeWeaponPrice)
			);


			// Ensure the widget is visible if it is not already.
			if (ShortRangeWeaponInformationWidgetInstance->IsVisible() == false)
			{
				ShortRangeWeaponInformationWidgetInstance->SetVisibility(ESlateVisibility::Visible);
			}

			// Hide the range weapon information widget if it is currently visible.
			if (LongRangeWeaponInformationWidgetInstance->IsVisible())
			{
				LongRangeWeaponInformationWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UCharacterUIComponent::UpdateLongRangeWeaponInformationWidget(AWeaponModule* InWeaponModule)
{
	if (InWeaponModule != nullptr)
	{
		// Check if the ShortRangeWeaponInformationWidgetInstance is valid (not null).
		if (LongRangeWeaponInformationWidgetInstance != nullptr)
		{
			/* Update the text fields of the widget with the retrieved properties of the short range weapon.*/

			// Weapon Name
			FString LongRangeWeaponName = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Name;
			LongRangeWeaponInformationWidgetInstance->Name->SetText
			(
				FText::FromString(LongRangeWeaponName)
			);

			// Weapon Rarity
			FString LongRangeWeaponRarityName = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Rarity;
			LongRangeWeaponInformationWidgetInstance->Rarity->SetText
			(
				FText::FromString(LongRangeWeaponRarityName)
			);

			// Weapon FireType
			FString LongRangeWeaponFireType = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().FireType;
			LongRangeWeaponInformationWidgetInstance->FireType->SetText
			(
				FText::FromString(LongRangeWeaponFireType)
			);

			// Weapon FireMode
			FString LongRangeWeaponFireMode = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().FireMode;
			LongRangeWeaponInformationWidgetInstance->FireMode->SetText
			(
				FText::FromString(LongRangeWeaponFireMode)
			);

			FString LongRangeWeaponAmmunitionType = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().AmmunitionTypeName;
			LongRangeWeaponInformationWidgetInstance->Ammunition->SetText
			(
				FText::FromString(LongRangeWeaponAmmunitionType)
			);

			// Weapon Damage
			FString LongRangeWeaponDamage = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Damage;
			LongRangeWeaponInformationWidgetInstance->Damage->SetText
			(
				FText::FromString(LongRangeWeaponDamage)
			);

			// Weapon Penetration
			FString LongRangeWeaponPenetration = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Penetration;
			LongRangeWeaponInformationWidgetInstance->Penetration->SetText
			(
				FText::FromString(LongRangeWeaponPenetration)
			);

			//Weapon Range
			FString LongRangeWeaponRange = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Range;
			LongRangeWeaponInformationWidgetInstance->Range->SetText
			(
				FText::FromString(LongRangeWeaponRange)
			);

			// Weapon Accuracy
			FString LongRangeWeaponAccuracy = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Accuracy;
			LongRangeWeaponInformationWidgetInstance->Accuracy->SetText
			(
				FText::FromString(LongRangeWeaponAccuracy)
			);

			// Weapon Magazine Capacity
			FString LongRangeWeaponMagCapacity = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().MagazineCapacity;
			LongRangeWeaponInformationWidgetInstance->MagazineCapacity->SetText
			(
				FText::FromString(LongRangeWeaponMagCapacity)
			);

			// Weapon Fire Rate
			FString LongRangeWeaponFireRate = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().FireRate;
			LongRangeWeaponInformationWidgetInstance->FireRate->SetText
			(
				FText::FromString(LongRangeWeaponFireRate)
			);

			// Weapon Reload Time
			FString LongRangeWeaponReloadTime = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().ReloadTime;
			LongRangeWeaponInformationWidgetInstance->ReloadTime->SetText
			(
				FText::FromString(LongRangeWeaponReloadTime)
			);

			// Weapon Weight
			FString LongRangeWeaponWeight = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Range;
			LongRangeWeaponInformationWidgetInstance->Weight->SetText
			(
				FText::FromString(LongRangeWeaponWeight)
			);

			// Weapon Durability
			FString LongRangeWeaponDurability = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Durability;
			LongRangeWeaponInformationWidgetInstance->Durability->SetText
			(
				FText::FromString(LongRangeWeaponDurability)
			);

			// Weapon Price
			FString LongRangeWeaponPrice = InWeaponModule->GetWeaponMasteryComponent()->GetWeaponDetails().Price;
			LongRangeWeaponInformationWidgetInstance->Price->SetText
			(
				FText::FromString(LongRangeWeaponPrice)
			);

			if (ShortRangeWeaponInformationWidgetInstance->IsVisible())
			{
				ShortRangeWeaponInformationWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
			}

			if (LongRangeWeaponInformationWidgetInstance->IsVisible() == false)
			{
				LongRangeWeaponInformationWidgetInstance->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UCharacterUIComponent::HideVisibleWeaponInformationWidgets()
{
	if (ShortRangeWeaponInformationWidgetInstance->IsVisible())
	{
		ShortRangeWeaponInformationWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}

	if (LongRangeWeaponInformationWidgetInstance->IsVisible())
	{
		LongRangeWeaponInformationWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}