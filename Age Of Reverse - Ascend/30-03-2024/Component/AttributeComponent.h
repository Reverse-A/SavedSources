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

private:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

	UPROPERTY()
	AAIEntityModule* AIEntityModule;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties|Attributes")
	float MaxHealth{ 100.0f };
	float Health;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties|Attributes")
	float MaxShield{ 100.0f };
	float Shield;

	UPROPERTY(EditAnywhere, Category = "DeveloperProperties|Attributes")
	float MaxStamina{ 100.0f };
	float Stamina;

public:
	float GetHealth() const;
	float GetMaxHealth() const;

	float GetShield() const;
	float GetMaxShield() const;

	float GetStamina() const;
	float GetMaxStamina() const;

	void SetHealth(float const NewHealth)	{ Health	= NewHealth; };
	void SetShield(float const NewShield)	{ Shield	= NewShield; };
	void SetStamina(float const NewStamina) { Stamina	= NewStamina; };

	void SetBaseCharacter(ABaseCharacter* Character)  { BaseCharacter = Character; }
	void SetAIEntityModule(AAIEntityModule* AIEntity) { AIEntityModule = AIEntity; }

};
