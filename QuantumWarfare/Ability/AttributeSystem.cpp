// @2023 All rights reversed by Reverse-Alpha Studios


#include "AttributeSystem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "QuantumWarfare/QWGameplayTags.h"
#include <QuantumWarfare/Interaction/CombatInterface.h>
#include <QuantumWarfare/PlayerController/Player/PlayerControllerModule.h>
#include <QuantumWarfare/Ability/BlueprintLibrary/PlayerAbilitySystemLibrary.h>

void UAttributeSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*Primary Attributes*/
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Strength,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Agility,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Dexterity,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Endurance,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Intelligence,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Perception,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Luck,			COND_None, REPNOTIFY_Always);

	/*Vital Attributes*/
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Health,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Shield,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Energy,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Stamina,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, MaxHealth,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, MaxShield,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, MaxEnergy,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, MaxStamina,	COND_None, REPNOTIFY_Always);

	/*Secondary Attributes*/
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Armor,					COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, ArmorPenetration,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, BlockChance,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, CriticalHitChance,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, CriticalHitDamage,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, MovementSpeed,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, AttackSpeed,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Evasion,				COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, Accuracy,				COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, StealthDetection,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, ResourceCostReduction, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, ThreatReduction,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, ExperienceGainBonus,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, ExplorationBonus,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, ItemDropBonus,			COND_None, REPNOTIFY_Always);

	/*Resistances*/
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, PhysicalResistance,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, BluntResistance,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, PierceResistance,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, CutResistance,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, PenetrationResistance, COND_None, REPNOTIFY_Always);

	/*Regenerations*/
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, HealthRegeneration,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, ShieldRegeneration,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, EnergyRegeneration,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSystem, StaminaRegeneration,	COND_None, REPNOTIFY_Always);

}

UAttributeSystem::UAttributeSystem()
{
	const FQWGameplayTags& GameplayTags = FQWGameplayTags::Get();

	/*Primary Attributes*/
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength,					GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Agility,					GetAgilityAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Dexterity,					GetDexterityAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Endurance,					GetEnduranceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence,				GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Perception,				GetPerceptionAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Luck,						GetLuckAttribute);

	/*Secondary Attributes*/
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor,					GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration,		GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance,				GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance,		GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage,		GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance,	GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MovementSpeed,			GetMovementSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_AttackSpeed,				GetAttackSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Evasion,					GetEvasionAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Accuracy,				GetAccuracyAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_StealthDetection,		GetStealthDetectionAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ResourceCostReduction,	GetResourceCostReductionAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ThreatReduction,			GetThreatReductionAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ExperienceGainBonus,		GetExperienceGainBonusAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ExplorationBonus,		GetExplorationBonusAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ItemDropBonus,			GetItemDropBonusAttribute);

	/*Resistance Attributes*/
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical,				GetPhysicalResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Blunt,					GetBluntResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Pierce,					GetPierceResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Cut,					GetCutResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Penetration,			GetPenetrationResistanceAttribute);

	/*Secondary Regeneration Attributes*/
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration,		GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ShieldRegeneration,		GetShieldRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_EnergyRegeneration,		GetEnergyRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_StaminaRegeneration,		GetStaminaRegenerationAttribute);
}

void UAttributeSystem::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());

	}

	if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());

	}

	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxEnergy());

	}

	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}

void UAttributeSystem::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect, (owner of this Attribute set)
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController  = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController	= Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter	= Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC			= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}



void UAttributeSystem::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		SetShield(FMath::Clamp(GetShield(), 0.0f, GetMaxShield()));
	}

	if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		SetEnergy(FMath::Clamp(GetEnergy(), 0.0f, GetMaxEnergy()));
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.0f);

		if (LocalIncomingDamage > 0.0f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			const bool bFatalDamage = NewHealth <= 0.0f;

			if (bFatalDamage)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Death();
				}
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FQWGameplayTags::Get().Effects_HitReaction);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			const bool bBlock		= UPlayerAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
			const bool bCriticalHit = UPlayerAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
			ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
		}
	}
}

void UAttributeSystem::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if (APlayerControllerModule* PCM = Cast<APlayerControllerModule>(Props.SourceCharacter->Controller))
		{
			PCM->ShowDamageNumber
			(
				Damage,
				Props.TargetCharacter,
				bBlockedHit,
				bCriticalHit
			);
		}
	}
}

#pragma region Primary-Attributes
void UAttributeSystem::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Strength, OldStrength);
}
void UAttributeSystem::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Agility, OldAgility);
}
void UAttributeSystem::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Dexterity, OldDexterity);
}
void UAttributeSystem::OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Endurance, OldEndurance);
}
void UAttributeSystem::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Intelligence, OldIntelligence);
}
void UAttributeSystem::OnRep_Perception(const FGameplayAttributeData& OldPerception) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Perception, OldPerception);
}
void UAttributeSystem::OnRep_Luck(const FGameplayAttributeData& OldLuck) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Luck, OldLuck);
}

#pragma endregion

#pragma region Secondary-Attributes
void UAttributeSystem::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Armor, OldArmor);
}

void UAttributeSystem::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, ArmorPenetration, OldArmorPenetration);
}

void UAttributeSystem::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, BlockChance, OldBlockChance);
}

void UAttributeSystem::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, CriticalHitChance, OldCriticalHitChance);

}

void UAttributeSystem::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, CriticalHitDamage, OldCriticalHitDamage);

}

void UAttributeSystem::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, CriticalHitResistance, OldCriticalHitResistance);

}

void UAttributeSystem::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, MovementSpeed, OldMovementSpeed);
}

void UAttributeSystem::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, AttackSpeed, OldAttackSpeed);
}

void UAttributeSystem::OnRep_Evasion(const FGameplayAttributeData& OldEvasion) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Evasion, OldEvasion);
}

void UAttributeSystem::OnRep_Accuracy(const FGameplayAttributeData& OldAccuracy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Accuracy, OldAccuracy);
}

void UAttributeSystem::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, HealthRegeneration, OldHealthRegeneration);
}

void UAttributeSystem::OnRep_ShieldRegeneration(const FGameplayAttributeData& OldShieldRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, ShieldRegeneration, OldShieldRegeneration);
}

void UAttributeSystem::OnRep_EnergyRegeneration(const FGameplayAttributeData& OldEnergyRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, EnergyRegeneration, OldEnergyRegeneration);
}

void UAttributeSystem::OnRep_StaminaRegeneration(const FGameplayAttributeData& OldStaminaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, StaminaRegeneration, OldStaminaRegeneration);
}

#pragma region Resistance
/*Physicals*/
void UAttributeSystem::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, PhysicalResistance, OldPhysicalResistance);
}

void UAttributeSystem::OnRep_BluntResistance(const FGameplayAttributeData& OldBluntResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, BluntResistance, OldBluntResistance);
}

void UAttributeSystem::OnRep_PierceResistance(const FGameplayAttributeData& OldPierceResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, PierceResistance, OldPierceResistance);
}

void UAttributeSystem::OnRep_CutResistance(const FGameplayAttributeData& OldCutResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, CutResistance, OldCutResistance);
}

void UAttributeSystem::OnRep_PenetrationResistance(const FGameplayAttributeData& OldPenetrationResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, PenetrationResistance, OldPenetrationResistance);
}

void UAttributeSystem::OnRep_StealthDetection(const FGameplayAttributeData& OldStealthDetection) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, StealthDetection, OldStealthDetection);
}

void UAttributeSystem::OnRep_ResourceCostReduction(const FGameplayAttributeData& OldResourceCostReduction) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, ResourceCostReduction, OldResourceCostReduction);
}

void UAttributeSystem::OnRep_ThreatReduction(const FGameplayAttributeData& OldThreatReduction) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, ThreatReduction, OldThreatReduction);
}

void UAttributeSystem::OnRep_ExperienceGainBonus(const FGameplayAttributeData& OldExperienceGainBonus) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, ExperienceGainBonus, OldExperienceGainBonus);
}
void UAttributeSystem::OnRep_ExplorationBonus(const FGameplayAttributeData& OldExplorationBonus) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, ExplorationBonus, OldExplorationBonus);
}

void UAttributeSystem::OnRep_ItemDropBonus(const FGameplayAttributeData& OldItemDropBonus) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, ItemDropBonus, OldItemDropBonus);
}

#pragma endregion


#pragma region Vital-Attributes

void UAttributeSystem::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Health, OldHealth);
}

void UAttributeSystem::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, MaxHealth, OldMaxHealth);
}

void UAttributeSystem::OnRep_Shield(const FGameplayAttributeData& OldShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Shield, OldShield);
}

void UAttributeSystem::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, MaxShield, OldMaxShield);
}

void UAttributeSystem::OnRep_Energy(const FGameplayAttributeData& OldEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Energy, OldEnergy);
}

void UAttributeSystem::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, MaxEnergy, OldMaxEnergy);
}

void UAttributeSystem::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, Stamina, OldStamina);
}

void UAttributeSystem::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSystem, MaxStamina, OldMaxStamina);
}
#pragma endregion
