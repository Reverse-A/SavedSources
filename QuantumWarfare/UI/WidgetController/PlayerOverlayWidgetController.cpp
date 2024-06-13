// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerOverlayWidgetController.h"
#include <QuantumWarfare/Ability/AttributeSystem.h>
#include <QuantumWarfare/Ability/AbilitySystem.h>

void UPlayerOverlayWidgetController::BroadcastInitialValues()
{
	const UAttributeSystem* PlayerAttributeSet = CastChecked<UAttributeSystem>(AttributeSet);

	OnHealthChanged.Broadcast(PlayerAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(PlayerAttributeSet->GetMaxHealth());

	OnShieldChanged.Broadcast(PlayerAttributeSet->GetShield());
	OnMaxShieldChanged.Broadcast(PlayerAttributeSet->GetMaxShield());

	OnEnergyChanged.Broadcast(PlayerAttributeSet->GetEnergy());
	OnMaxEnergyChanged.Broadcast(PlayerAttributeSet->GetMaxEnergy());

	OnStaminaChanged.Broadcast(PlayerAttributeSet->GetStamina());
	OnMaxStaminaChanged.Broadcast(PlayerAttributeSet->GetMaxStamina());


}

void UPlayerOverlayWidgetController::BindCallbacksDependencies()
{
	const UAttributeSystem* PlayerAttributeSet = CastChecked<UAttributeSystem>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetHealthAttribute())
	.AddLambda
	(
		[this]
		(const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetMaxHealthAttribute())
		.AddLambda
		(
			[this]
			(const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetShieldAttribute())
		.AddLambda
		(
			[this]
			(const FOnAttributeChangeData& Data)
			{
				OnShieldChanged.Broadcast(Data.NewValue);
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetMaxShieldAttribute())
		.AddLambda
		(
			[this]
			(const FOnAttributeChangeData& Data)
			{
				OnMaxShieldChanged.Broadcast(Data.NewValue);
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetEnergyAttribute())
		.AddLambda
		(
			[this]
			(const FOnAttributeChangeData& Data)
			{
				OnEnergyChanged.Broadcast(Data.NewValue);
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetMaxEnergyAttribute())
		.AddLambda
		(
			[this]
			(const FOnAttributeChangeData& Data)
			{
				OnMaxEnergyChanged.Broadcast(Data.NewValue);
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetStaminaAttribute())
		.AddLambda
		(
			[this]
			(const FOnAttributeChangeData& Data)
			{
				OnStaminaChanged.Broadcast(Data.NewValue);
			}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PlayerAttributeSet->GetMaxStaminaAttribute())
		.AddLambda
		(
			[this]
			(const FOnAttributeChangeData& Data)
			{
				OnMaxStaminaChanged.Broadcast(Data.NewValue);
			}
	);

	Cast<UAbilitySystem>(AbilitySystemComponent)->EffectAssetTags.AddLambda
	(
		[this]
		(const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// For example, say that Tag = Message.HealthRegeneration				
				// "Message.HealthPotion". MatchesTag("Message") will return True, "Message". MatchesTag("Message.HealthRegeneration") will return False
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
}







/*
AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
(PlayerAttributeSet->GetHealthAttribute()).AddUObject(this, &UPlayerOverlayWidgetController::HealthChanged);

AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
(PlayerAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UPlayerOverlayWidgetController::MaxHealthChanged);

AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
(PlayerAttributeSet->GetShieldAttribute()).AddUObject(this, &UPlayerOverlayWidgetController::ShieldChanged);

AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
(PlayerAttributeSet->GetMaxShieldAttribute()).AddUObject(this, &UPlayerOverlayWidgetController::MaxShieldChanged);

AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
(PlayerAttributeSet->GetEnergyAttribute()).AddUObject(this, &UPlayerOverlayWidgetController::EnergyChanged);

AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
(PlayerAttributeSet->GetMaxEnergyAttribute()).AddUObject(this, &UPlayerOverlayWidgetController::MaxEnergyChanged);

AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
(PlayerAttributeSet->GetStaminaAttribute()).AddUObject(this, &UPlayerOverlayWidgetController::StaminaChanged);

AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
(PlayerAttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &UPlayerOverlayWidgetController::MaxStaminaChanged);




void HealthChanged(const FOnAttributeChangeData& Data) const;
void MaxHealthChanged(const FOnAttributeChangeData& Data) const;

void ShieldChanged(const FOnAttributeChangeData& Data) const;
void MaxShieldChanged(const FOnAttributeChangeData& Data) const;

void EnergyChanged(const FOnAttributeChangeData& Data) const;
void MaxEnergyChanged(const FOnAttributeChangeData& Data) const;

void StaminaChanged(const FOnAttributeChangeData& Data) const;
void MaxStaminaChanged(const FOnAttributeChangeData& Data) const;


void UPlayerOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UPlayerOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UPlayerOverlayWidgetController::ShieldChanged(const FOnAttributeChangeData& Data) const
{
	OnShieldChanged.Broadcast(Data.NewValue);
}

void UPlayerOverlayWidgetController::MaxShieldChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxShieldChanged.Broadcast(Data.NewValue);
}

void UPlayerOverlayWidgetController::EnergyChanged(const FOnAttributeChangeData& Data) const
{
	OnEnergyChanged.Broadcast(Data.NewValue);
}

void UPlayerOverlayWidgetController::MaxEnergyChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxEnergyChanged.Broadcast(Data.NewValue);
}

void UPlayerOverlayWidgetController::StaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaChanged.Broadcast(Data.NewValue);
}

void UPlayerOverlayWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);
}
*/