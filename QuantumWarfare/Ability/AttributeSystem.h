// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributeSystem.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

// typedef is specific to the FGameplayAttribute() signature, but TStaticFunPtr is generic to any signature chosen.
// typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class QUANTUMWARFARE_API UAttributeSystem : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAttributeSystem();

	virtual void GetLifetimeReplicatedProps	(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange			(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute	(const FGameplayEffectModCallbackData& Data) override;


	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

#pragma region Primary-Attributes
	/*Strength*/
public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Strength);

	/*Agility*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Primary Attributes")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Agility);

	/*Dexterity*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Dexterity);

	/*Endurance*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Endurance, Category = "Primary Attributes")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Endurance);

	/*Intelligence*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Intelligence);

	/*Perception*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Perception, Category = "Primary Attributes")
	FGameplayAttributeData Perception;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Perception);

	/*Luck*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Luck, Category = "Primary Attributes")
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Luck);
#pragma endregion

#pragma region Secondary-Attributes
/*Armor*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Primary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Armor);

/*Armor Penetration*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, ArmorPenetration);

/*Block Chance*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, BlockChance);

/*CriticalHitChance*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, CriticalHitChance);

/*CriticalHitDamage*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, CriticalHitDamage);

/*CriticalHitResistance*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, CriticalHitResistance);

/*Movement Speed*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Secondary Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, MovementSpeed);

/*Attack Speed*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Secondary Attributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, AttackSpeed);

/*Evasion*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Evasion, Category = "Secondary Attributes")
	FGameplayAttributeData Evasion;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Evasion);

/*Accuracy*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Accuracy, Category = "Secondary Attributes")
	FGameplayAttributeData Accuracy;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Accuracy);

/*Stealth Detection*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StealthDetection, Category = "Secondary Attributes")
	FGameplayAttributeData StealthDetection;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, StealthDetection);

/*Resource cost reduction*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResourceCostReduction, Category = "Secondary Attributes")
	FGameplayAttributeData ResourceCostReduction;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, ResourceCostReduction);

/*Threat Reduction*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ThreatReduction, Category = "Secondary Attributes")
	FGameplayAttributeData ThreatReduction;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, ThreatReduction);

/*Experience Gain Bonus*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ExperienceGainBonus, Category = "Secondary Attributes")
	FGameplayAttributeData ExperienceGainBonus;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, ExperienceGainBonus);

/*Exploration Bonuses*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ExplorationBonus, Category = "Secondary Attributes")
	FGameplayAttributeData ExplorationBonus;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, ExplorationBonus);

/*Rare Item Drop Rate Increase*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ItemDropBonus, Category = "Secondary Attributes")
	FGameplayAttributeData ItemDropBonus;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, ItemDropBonus);

#pragma region AttrRegeneration
	/*HealthRegeneration*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, HealthRegeneration);

	/*Shield Regeneration*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShieldRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ShieldRegeneration;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, ShieldRegeneration);

	/*Energy Regeneration*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EnergyRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData EnergyRegeneration;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, EnergyRegeneration);

	/*Stamina Regeneration*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData StaminaRegeneration;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, StaminaRegeneration);

#pragma region Resistance-Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, PhysicalResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BluntResistance, Category = "Resistance Attributes")
	FGameplayAttributeData BluntResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, BluntResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PierceResistance, Category = "Resistance Attributes")
	FGameplayAttributeData PierceResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, PierceResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CutResistance, Category = "Resistance Attributes")
	FGameplayAttributeData CutResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, CutResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PenetrationResistance, Category = "Resistance Attributes")
	FGameplayAttributeData PenetrationResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, PenetrationResistance);

#pragma endregion
/*Primary */
	UFUNCTION()	void OnRep_Strength					(const FGameplayAttributeData& OldStrength)					const;
	UFUNCTION()	void OnRep_Agility					(const FGameplayAttributeData& OldAgility)					const;
	UFUNCTION()	void OnRep_Dexterity				(const FGameplayAttributeData& OldDexterity)				const;
	UFUNCTION()	void OnRep_Endurance				(const FGameplayAttributeData& OldEndurance)				const;
	UFUNCTION()	void OnRep_Intelligence				(const FGameplayAttributeData& OldIntelligence)				const;
	UFUNCTION()	void OnRep_Perception				(const FGameplayAttributeData& OldPerception)				const;
	UFUNCTION()	void OnRep_Luck						(const FGameplayAttributeData& OldLuck)						const;

/*Secondary*/
	UFUNCTION()	void OnRep_Armor					(const FGameplayAttributeData& OldArmor)					const;
	UFUNCTION()	void OnRep_ArmorPenetration			(const FGameplayAttributeData& OldArmorPenetration)			const;
	UFUNCTION()	void OnRep_BlockChance				(const FGameplayAttributeData& OldBlockChance)				const;
	UFUNCTION()	void OnRep_CriticalHitChance		(const FGameplayAttributeData& OldCriticalHitChance)		const;
	UFUNCTION()	void OnRep_CriticalHitDamage		(const FGameplayAttributeData& OldCriticalHitDamage)		const;
	UFUNCTION()	void OnRep_CriticalHitResistance	(const FGameplayAttributeData& OldCriticalHitResistance)	const;
	UFUNCTION()	void OnRep_MovementSpeed			(const FGameplayAttributeData& OldMovementSpeed)			const;
	UFUNCTION()	void OnRep_AttackSpeed				(const FGameplayAttributeData& OldAttackSpeed)				const;
	UFUNCTION()	void OnRep_Evasion					(const FGameplayAttributeData& OldEvasion)					const;
	UFUNCTION()	void OnRep_Accuracy					(const FGameplayAttributeData& OldAccuracy)					const;
	UFUNCTION()	void OnRep_StealthDetection			(const FGameplayAttributeData& OldStealthDetection)			const;
	UFUNCTION()	void OnRep_ResourceCostReduction	(const FGameplayAttributeData& OldResourceCostReduction)	const;
	UFUNCTION()	void OnRep_ThreatReduction			(const FGameplayAttributeData& OldThreatReduction)			const;
	UFUNCTION()	void OnRep_ExperienceGainBonus		(const FGameplayAttributeData& OldExperienceGainBonus)		const;
	UFUNCTION()	void OnRep_ExplorationBonus			(const FGameplayAttributeData& OldExplorationBonuses)		const;
	UFUNCTION()	void OnRep_ItemDropBonus			(const FGameplayAttributeData& OldItemDropBonuses)			const;
	UFUNCTION()	void OnRep_HealthRegeneration		(const FGameplayAttributeData& OldHealthRegeneration)		const;
	UFUNCTION()	void OnRep_ShieldRegeneration		(const FGameplayAttributeData& OldShieldRegeneration)		const;
	UFUNCTION()	void OnRep_EnergyRegeneration		(const FGameplayAttributeData& OldEnergyRegeneration)		const;
	UFUNCTION()	void OnRep_StaminaRegeneration		(const FGameplayAttributeData& OldStaminaRegeneration)		const;

/*Resistance*/
	/*Physicals*/
	UFUNCTION() void OnRep_PhysicalResistance		(const FGameplayAttributeData& OldPhysicalResistance)		const;
	UFUNCTION() void OnRep_BluntResistance			(const FGameplayAttributeData& OldBluntResistance)			const;
	UFUNCTION() void OnRep_PierceResistance			(const FGameplayAttributeData& OldPierceResistance)			const;
	UFUNCTION() void OnRep_CutResistance			(const FGameplayAttributeData& OldCutResistance)			const;
	UFUNCTION() void OnRep_PenetrationResistance	(const FGameplayAttributeData& OldPenetrationResistance)	const;


#pragma endregion Server-ReplicationFunctions
#pragma endregion Regeneration-Attributes

#pragma endregion

#pragma region Vital-Attributes
/*Health*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, MaxHealth);
/*Shield*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "Vital Attributes")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Shield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxShield, Category = "Vital Attributes")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, MaxShield);

/*Energy*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Energy, Category = "Vital Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Energy);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy, Category = "Vital Attributes")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, MaxEnergy);

/*Stamina*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, Stamina);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Vital Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UAttributeSystem, MaxStamina);

	UFUNCTION()	void OnRep_Health			(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()	void OnRep_MaxHealth		(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()	void OnRep_Shield			(const FGameplayAttributeData& OldShield) const;
	UFUNCTION()	void OnRep_MaxShield		(const FGameplayAttributeData& OldMaxShield) const;
	UFUNCTION()	void OnRep_Energy			(const FGameplayAttributeData& OldEnergy) const;
	UFUNCTION()	void OnRep_MaxEnergy		(const FGameplayAttributeData& OldMaxEnergy) const;
	UFUNCTION()	void OnRep_Stamina			(const FGameplayAttributeData& OldStamina) const;
	UFUNCTION()	void OnRep_MaxStamina		(const FGameplayAttributeData& OldMaxStamina) const;


#pragma endregion

#pragma region Meta Attributes
public:
	UPROPERTY(BlueprintReadOnly, Category = "MetaAttributes")
	FGameplayAttributeData IncomingDamage;

	ATTRIBUTE_ACCESSORS(UAttributeSystem, IncomingDamage)

#pragma endregion

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
#pragma endregion

};
