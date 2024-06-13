// @2023 All rights reversed by Reverse-Alpha Studios


#include "QWGameplayTags.h"
#include "GameplayTagsManager.h"

FQWGameplayTags FQWGameplayTags::GameplayTags;

void FQWGameplayTags::InitializeNativeGameplayTags()
{
#pragma region Primary Attributes
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Primary.Strength"),
		FString("Determines the character's physical power, impacting melee attack damage, carrying capacity, and the ability to wield heavy weapons effectively.")
	);

	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Primary.Intelligence"),
		FString("Reflects the character's mental acuity and problem-solving abilities, influencing critical hit chance, spell effectiveness (if applicable), and tactical decision-making.")
	);

	GameplayTags.Attributes_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Primary.Agility"),
		FString("Enhances the character's speed and agility, affecting movement speed, evasion, and reaction time.")
	);

	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Primary.Dexterity"),
		FString("Governs the character's precision and fine motor skills, influencing accuracy, reload speed, and melee attack speed.")
	);

	GameplayTags.Attributes_Primary_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Primary.Endurance"),
		FString("Represents the character's stamina and resilience, affecting maximum health, resistance to damage over time effects, and the ability to withstand prolonged battles.")
	);

	GameplayTags.Attributes_Primary_Perception = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Primary.Perception"),
		FString("Governs the character's awareness and sensory perception, affecting detection of hidden enemies, critical hit chance, and accuracy at long range.")
	);

	GameplayTags.Attributes_Primary_Luck = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Primary.Luck"),
		FString("Represents the character's fortune and chance, influencing critical hit chance, loot quality, and the likelihood of beneficial random events.")
	);
#pragma endregion


#pragma region Secondary Attributes
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Determines the maximum amount of health points a character can have.")
	);

	GameplayTags.Attributes_Secondary_MaxShield = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.MaxShield"),
		FString("Determines the maximum amount of shield points a character can have.")
	);

	GameplayTags.Attributes_Secondary_MaxEnergy = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.MaxEnergy"),
		FString("Determines the maximum amount of energy points a character can have.")
	);

	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.MaxStamina"),
		FString("Determines the maximum amount of stamina points a character can have.")
	);

	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces incoming damage.")
	);

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Increases damage against armored targets.")
	);

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to block incoming attacks.")
	);

	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to deal critical hits.")
	);

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Increased damage on critical hits.")
	);

	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduced damage on critical hits.")
	);

	GameplayTags.Attributes_Secondary_MovementSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.MovementSpeed"),
		FString("Speed of character movement.")
	);

	GameplayTags.Attributes_Secondary_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.AttackSpeed"),
		FString("Speed of character attacks.")
	);

	GameplayTags.Attributes_Secondary_Evasion = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.Evasion"),
		FString("Chance to evade incoming attacks.")
	);

	GameplayTags.Attributes_Secondary_Accuracy = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.Accuracy"),
		FString("Accuracy of character attacks.")
	);

	GameplayTags.Attributes_Secondary_StealthDetection = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.StealthDetection"),
		FString("Increase stealth capability and reduce detection.")
	);

	GameplayTags.Attributes_Secondary_ResourceCostReduction = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.ResourceCostReduction"),
		FString("Reduction in resource costs for actions.")
	);

	GameplayTags.Attributes_Secondary_ThreatReduction = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.ThreatReduction"),
		FString("Reduction in threat level, minimizing enemy aggression.")
	);

	GameplayTags.Attributes_Secondary_ExperienceGainBonus = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.ExperienceGainBonus"),
		FString("Bonus to experience gained from activities.")
	);

	GameplayTags.Attributes_Secondary_ExplorationBonus = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.ExplorationBonus"),
		FString("Bonus to rewards obtained from exploration.")
	);

	GameplayTags.Attributes_Secondary_ItemDropBonus = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.ItemDropBonus"),
		FString("Bonus to the drop rate of items from enemies.")
	);

	/*Regeneration*/
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Rate at which health regenerates over time.")
	);

	GameplayTags.Attributes_Secondary_ShieldRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.ShieldRegeneration"),
		FString("Rate at which shield regenerates over time.")
	);

	GameplayTags.Attributes_Secondary_EnergyRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.EnergyRegeneration"),
		FString("Rate at which energy regenerates over time.")
	);

	GameplayTags.Attributes_Secondary_StaminaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Secondary.StaminaRegeneration"),
		FString("Rate at which stamina regenerates over time.")
	);

#pragma endregion

#pragma region InputTags
	/*MouseButtons*/
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.LMB"),
		FString("Input tag for left mouse button.")
	);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.RMB"),
		FString("Input tag for right mouse button.")
	);

	GameplayTags.InputTag_Interaction = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.Interaction"),
		FString("Input tag for interaction.")
	);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.1"),
		FString("Input tag for right mouse button.")
	);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.2"),
		FString("Input tag for right mouse button.")
	);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.3"),
		FString("Input tag for right mouse button.")
	);

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("InputTag.4"),
		FString("Input tag for right mouse button.")
	);


#pragma region Specify Later
	GameplayTags.Damage				= UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Damage"),
		FString("Damage.")
	);

#pragma region DamageTypes - General
#pragma region Physicals
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Damage.Physical"),
		FString("Physical Damage Type")
	);

	GameplayTags.Damage_Blunt = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Damage.Blunt"),
		FString("Blunt Damage Type.")
	);

	GameplayTags.Damage_Pierce = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Damage.Pierce"),
		FString("Pierce Damage Type.")
	);

	GameplayTags.Damage_Cut = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Damage.Cut"),
		FString("Cut Damage Type.")
	);

	GameplayTags.Damage_Penetration = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Damage.Penetration"),
		FString("Penetration Damage Type.")
	);
/*Resistances*/
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Resistance.Physical"),
		FString("Attributes Resistance Physical")
	);

	GameplayTags.Attributes_Resistance_Blunt = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Resistance.Blunt"),
		FString("Attributes Resistance to Blunt")
	);

	GameplayTags.Attributes_Resistance_Pierce = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Resistance.Pierce"),
		FString("Attributes Resistance to Pierce")
	);

	GameplayTags.Attributes_Resistance_Cut = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Resistance.Cut"),
		FString("Attributes Resistance to Cut")
	);

	GameplayTags.Attributes_Resistance_Penetration = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Attributes.Resistance.Penetration"),
		FString("Attributes Resistance to Penetration")
	);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical,		GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Blunt,		GameplayTags.Attributes_Resistance_Blunt);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Pierce,		GameplayTags.Attributes_Resistance_Pierce);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Cut,			GameplayTags.Attributes_Resistance_Cut);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Penetration,	GameplayTags.Attributes_Resistance_Penetration);

#pragma endregion
#pragma endregion 



#pragma endregion


/*Hits*/
	GameplayTags.Effects_HitReaction = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Effects.HitReaction"),
		FString("Tag granted when hit reacting")
	);
#pragma endregion
}