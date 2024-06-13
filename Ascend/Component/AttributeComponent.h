// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

class ABaseCharacter;
class AAIEntityModule;

class UWidgetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCEND_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

#pragma region Classes
private:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

	UPROPERTY()
	AAIEntityModule* AIEntityModule;

public:
	void SetBaseCharacter(ABaseCharacter* Character)  { BaseCharacter = Character; }
	void SetAIEntityModule(AAIEntityModule* AIEntity) { AIEntityModule = AIEntity; }
#pragma endregion

#pragma region PlayerLevel
	int32 PlayerLevel;

	void SetPlayerLevel(int32 PlayerCurrentLevel)
	{
		PlayerLevel = PlayerCurrentLevel;
	}
#pragma endregion

#pragma region Player-Attributes
/*PhysicalConditions*/
private:
	float PlayerMaxInitialHealth { 50.0f };
	float PlayerInitialMaxShield { 50.0f };
	float PlayerInitialMaxStamina{ 50.0f };


	UPROPERTY(EditAnywhere, Category = "DeveloperProperties|Attributes")
	float PlayerMaxHealth{ PlayerMaxInitialHealth };
	float PlayerHealth = PlayerMaxHealth;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties|Attributes")
	float PlayerMaxShield{ PlayerInitialMaxShield };
	float PlayerShield = PlayerMaxShield;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties|Attributes")
	float PlayerMaxStamina{ PlayerInitialMaxStamina };
	float PlayerStamina = PlayerMaxStamina;


/*Functions*/
public:
	/*Stamina*/
	void DecreaseStamina();
	void IncreaseStamina();

private:
	float StaminaDecreaseAmount();
	float StaminaIncreaseAmount();
public:
/*Getters*/
	float GetPlayerHealth() const;
	float GetPlayerMaxHealth() const;

	float GetPlayerShield() const;
	float GetPlayerMaxShield() const;

	float GetPlayerStamina() const;
	float GetPlayerMaxStamina() const;

/*Setters*/
	void SetPlayerHealth  (float const NewHealth)	
	{ 
		PlayerHealth	= NewHealth;
	};

	void SetPlayerShield  (float const NewShield)	 
	{ 
		PlayerShield	= NewShield;
	};
	void SetPlayerStamina (float const NewStamina)	 
	{
		PlayerStamina	= NewStamina;
	};

	bool PlayerAlive()
	{
		return PlayerHealth > 0;
	}

#pragma endregion

#pragma region AI-Attributes
/*PhysicalConditions*/
private:
	float AIMaxInitialHealth	{ 50.0f };
	float AIMaxInitialShield	{ 50.0f };
	float AIMaxInitialStamina	{ 50.0f };

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties|Attributes")
	float AIMaxHealth{ AIMaxInitialHealth };
	float AIHealth = AIMaxHealth;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties|Attributes")
	float AIMaxShield{ AIMaxInitialShield };
	float AIShield = AIMaxShield;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties|Attributes")
	float AIMaxStamina{ AIMaxInitialStamina };
	float AIStamina = AIMaxStamina;

public:
	float GetAIHealth()		const;
	float GetAIMaxHealth()	const;

	float GetAIShield()		const;
	float GetAIMaxShield()	const;

	float GetAIStamina()	const;
	float GetAIMaxStamina() const;

	void SetAIHealth  (float const NewHealth)  
	{
		AIHealth  = NewHealth;
	};

	void SetAIShield  (float const NewShield)   
	{
		AIShield  = NewShield;
	};

	void SetAIStamina (float const NewStamina) 
	{ 
		AIStamina = NewStamina;
	};

#pragma endregion
};
