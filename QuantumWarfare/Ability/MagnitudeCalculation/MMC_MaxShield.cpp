// @2023 All rights reversed by Reverse-Alpha Studios


#include "MMC_MaxShield.h"
#include "QuantumWarfare/Ability/AttributeSystem.h"
#include <QuantumWarfare/Interaction/CombatInterface.h>

UMMC_MaxShield::UMMC_MaxShield()
{
	EnduranceDef.AttributeToCapture = UAttributeSystem::GetEnduranceAttribute();
	EnduranceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EnduranceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(EnduranceDef);

	DexterityDef.AttributeToCapture = UAttributeSystem::GetDexterityAttribute();
	DexterityDef.AttributeSource	= EGameplayEffectAttributeCaptureSource::Target;
	DexterityDef.bSnapshot			= false;

	RelevantAttributesToCapture.Add(DexterityDef);
}

float UMMC_MaxShield::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Endurance = 0.0f;
	GetCapturedAttributeMagnitude(EnduranceDef, Spec, EvaluationParameters, Endurance);
	Endurance = FMath::Max<float>(Endurance, 0.0f);

	float Dexterity = 0.0f;
	GetCapturedAttributeMagnitude(DexterityDef, Spec, EvaluationParameters, Dexterity);
	Dexterity = FMath::Max<float>(Dexterity, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();



	/*Calculation Method*/
	// EnduranceInitialize = 10.0f;
	// 50 + 2 * 10 + 10.0f * 1; 
	return ((Dexterity + Endurance) * 2.0f) + (2.0f * PlayerLevel);
}
