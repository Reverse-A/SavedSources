// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 * QuantumWarfare GameplayTags
 * Singleton containing native Gameplay Tags
 */

struct FQWGameplayTags
{
public:
	static const FQWGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	/*Native Primary GameplayTags*/
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Agility;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Endurance;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Perception;
	FGameplayTag Attributes_Primary_Luck;

	/*Native Secondary GameplayTags*/
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxShield;
	FGameplayTag Attributes_Secondary_MaxEnergy;
	FGameplayTag Attributes_Secondary_MaxStamina;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_MovementSpeed;
	FGameplayTag Attributes_Secondary_AttackSpeed;
	FGameplayTag Attributes_Secondary_Evasion;
	FGameplayTag Attributes_Secondary_Accuracy;
	FGameplayTag Attributes_Secondary_StealthDetection;
	FGameplayTag Attributes_Secondary_ResourceCostReduction;
	FGameplayTag Attributes_Secondary_ThreatReduction;
	FGameplayTag Attributes_Secondary_ExperienceGainBonus;
	FGameplayTag Attributes_Secondary_ExplorationBonus;
	FGameplayTag Attributes_Secondary_ItemDropBonus;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ShieldRegeneration;
	FGameplayTag Attributes_Secondary_EnergyRegeneration;
	FGameplayTag Attributes_Secondary_StaminaRegeneration;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_Interaction;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	FGameplayTag Damage;

/*DamageTypes*/
/*Physical damages and resistances*/
	// Damage caused by projectiles, such as bullets from firearms.
	FGameplayTag Damage_Physical;
	// Damage from blunt objects or force, such as hammers, clubs, or punches.
	FGameplayTag Damage_Blunt;
	// Damage from sharp objects piercing the body, like arrows or spears.
	FGameplayTag Damage_Pierce;
	// Damage from cutting or slashing with sharp objects, such as knives or swords.
	FGameplayTag Damage_Cut;
	// Severe internal injuries caused by objects penetrating deep into the body.
	FGameplayTag Damage_Penetration;

	FGameplayTag Attributes_Resistance_Physical;
	FGameplayTag Attributes_Resistance_Blunt;
	FGameplayTag Attributes_Resistance_Pierce;
	FGameplayTag Attributes_Resistance_Cut;
	FGameplayTag Attributes_Resistance_Penetration;

/*Elemental damages and resistances*/
	// Damage from fire or high temperatures, causing burns.
	FGameplayTag Damage_Fire;
	// Damage from extreme cold, causing frostbite or freezing.
	FGameplayTag Damage_Ice;
	// Damage from electrical sources, causing burns or cardiac arrest.
	FGameplayTag Damage_Electric;
	// Damage from corrosive substances, like strong acids or bases.
	FGameplayTag Damage_Corrosive;
	// Damage from exposure to radiation, causing burns, sickness, or genetic damage.
	FGameplayTag Damage_Radiation;
	// General damage from extreme heat or cold, not specifically fire or ice.
	FGameplayTag Damage_Thermal;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Ice;
	FGameplayTag Attributes_Resistance_Electric;
	FGameplayTag Attributes_Resistance_Corrosive;
	FGameplayTag Attributes_Resistance_Radiation;
	FGameplayTag Attributes_Resistance_Thermal;

/* Energy damages and resistance */
	// Damage from high-energy plasma, causing burns or disintegration.
	FGameplayTag Damage_Plasma;
	// Damage from high-frequency microwave radiation.
	FGameplayTag Damage_Microwave;
	// Damage from intense sound waves or vibrations.
	FGameplayTag Damage_Sonic;
	// Damage from focused light beams, causing burns or cutting.
	FGameplayTag Damage_Laser;
	// Damage from explosive forces, causing burns, concussions, or shrapnel wounds.
	FGameplayTag Damage_Explosive;

	FGameplayTag Attributes_Resistance_Plasma;
	FGameplayTag Attributes_Resistance_Microwave;
	FGameplayTag Attributes_Resistance_Sonic;
	FGameplayTag Attributes_Resistance_Laser;
	FGameplayTag Attributes_Resistance_Explosive;

/* Chemical/Biological damages and resistance */
	// Damage from toxic substances that poison the body.
	FGameplayTag Damage_Poison;
	// Damage from acidic substances causing burns and tissue damage.
	FGameplayTag Damage_Acid;
	// General damage from toxic substances that harm the body on contact or inhalation.
	FGameplayTag Damage_Toxic;
	// Damage over time caused by bacterial, viral, or fungal infections.
	FGameplayTag Damage_Infection;
	// Damage that includes paralytic effects, typically from toxins or poisons causing paralysis.
	FGameplayTag Damage_Paralytic;

	FGameplayTag Attributes_Resistance_Poison;
	FGameplayTag Attributes_Resistance_Acid;
	FGameplayTag Attributes_Resistance_Toxic;
	FGameplayTag Attributes_Resistance_Infection;
	FGameplayTag Attributes_Resistance_Paralytic;

/* Psychological damages and resistance */
	// Psychological damage resulting from extremely stressful or traumatic events.
	FGameplayTag Damage_Trauma;
	// Damage from exposure to substances that cause hallucinations and potential psychological harm.
	FGameplayTag Damage_Hallucinogenic;

	FGameplayTag Attributes_Resistance_Trauma;
	FGameplayTag Attributes_Resistance_Hallucinogenic;



	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	FGameplayTag Effects_HitReaction;

private:
	static FQWGameplayTags GameplayTags;
};