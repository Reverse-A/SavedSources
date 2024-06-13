// @2023 All rights reversed by Reverse-Alpha Studios


#include "AttributeComponent.h"

#include "Components/WidgetComponent.h"

#include "Ascend/AI/AIEntityModule.h"
#include "Ascend/Constant/Constants.h"
#include "Ascend/Widget/HealthBar.h"

static FName NAME_HealthValue(TEXT("HealthValue"));

UAttributeComponent::UAttributeComponent()
:	Health{MaxHealth},
	Shield{MaxShield},
	Stamina{MaxStamina}
{
	PrimaryComponentTick.bCanEverTick = false;
}


float UAttributeComponent::GetHealth() const
{
	return Health;
}

float UAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UAttributeComponent::GetShield() const
{
	return Shield;
}

float UAttributeComponent::GetMaxShield() const
{
	return MaxShield;
}

float UAttributeComponent::GetStamina() const
{
	return Stamina;
}

float UAttributeComponent::GetMaxStamina() const
{
	return MaxStamina;
}
