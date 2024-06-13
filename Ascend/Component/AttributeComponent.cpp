// @2023 All rights reversed by Reverse-Alpha Studios


#include "AttributeComponent.h"

#include "Components/WidgetComponent.h"

#include "Ascend/AI/AIEntityModule.h"
#include "Ascend/Constant/Constants.h"
#include "Ascend/Widget/HealthBar.h"

static FName NAME_HealthValue(TEXT("HealthValue"));

UAttributeComponent::UAttributeComponent()
:	PlayerHealth{PlayerMaxHealth},
	PlayerShield{PlayerMaxShield},
	PlayerStamina{PlayerMaxStamina}
{
	PrimaryComponentTick.bCanEverTick = false;
}


float UAttributeComponent::GetPlayerHealth() const
{
	return PlayerHealth;
}

float UAttributeComponent::GetPlayerMaxHealth() const
{
	return PlayerMaxHealth;
}

float UAttributeComponent::GetPlayerShield() const
{
	return PlayerShield;
}

float UAttributeComponent::GetPlayerMaxShield() const
{
	return PlayerMaxShield;
}

float UAttributeComponent::GetPlayerStamina() const
{
	return PlayerStamina;
}

float UAttributeComponent::GetPlayerMaxStamina() const
{
	return PlayerMaxStamina;
}
#pragma endregion

#pragma region PlayerStamina

void UAttributeComponent::DecreaseStamina()
{
	if (PlayerStamina <= 0.0f)
	{
		return;
	}

	PlayerStamina -= StaminaDecreaseAmount();
}

void UAttributeComponent::IncreaseStamina()
{
	if (PlayerStamina >= PlayerMaxStamina)
	{
		return;
	}

	PlayerStamina += StaminaIncreaseAmount();
}

float UAttributeComponent::StaminaDecreaseAmount()
{
	// Get Max player level
	int32 MaxPlayerLevel						= 50;
	// Set Initial stamina decrease amount
	float BaseDecreaseAmount					= 5.0f;
	// Max player level stamina decrease amount 
	float MaxPlayerLevelStaminaDecreaseAmount	= 2.5f;

	// Calculate the decrease amount based on the player's level
	float DegenerationStaminaAmount = FMath::Lerp
	(
		BaseDecreaseAmount,
		MaxPlayerLevelStaminaDecreaseAmount,
		static_cast<float>(PlayerLevel) / MaxPlayerLevel
	);

	// Multiple by delta time to make it slower 
	float Multiplier = 2.0f;
	float MultiDeltaSeconds = GetWorld()->GetDeltaSeconds() * Multiplier ;
	DegenerationStaminaAmount *= MultiDeltaSeconds;

	// TotalResult
	return DegenerationStaminaAmount;
}

float UAttributeComponent::StaminaIncreaseAmount()
{
	// Define the base regeneration amount
	float BaseStaminaRegenerationAmount = 5.0f;

	// Define the max level and the regeneration amount at max level
	int32 MaxPlayerLevel = 50;
	// Set the max regeneration of stamina amount
	float MaxPlayerLevelStaminaRegenerationAmount = 10.0f;

	// Calculate the regeneration amount based on the player's level
	float RegenerationStaminaAmount = FMath::Lerp
	(
		BaseStaminaRegenerationAmount,
		MaxPlayerLevelStaminaRegenerationAmount,
		static_cast<float>(PlayerLevel) / MaxPlayerLevel
	);

	// Multiple by delta time to make it slower 
	float Multiplier = 2.0f;
	float MultiDeltaSeconds = GetWorld()->GetDeltaSeconds() * Multiplier;
	RegenerationStaminaAmount *= MultiDeltaSeconds;

	// TotalResult
	return RegenerationStaminaAmount;
}
#pragma endregion



#pragma region AI-Attributes

float UAttributeComponent::GetAIHealth() const
{
	return AIHealth;
}

float UAttributeComponent::GetAIMaxHealth() const
{
	return AIMaxHealth;
}

float UAttributeComponent::GetAIShield() const
{
	return AIShield;
}

float UAttributeComponent::GetAIMaxShield() const
{
	return AIMaxShield;
}

float UAttributeComponent::GetAIStamina() const
{
	return AIStamina;
}

float UAttributeComponent::GetAIMaxStamina() const
{
	return AIMaxStamina;
}
#pragma endregion
