// @2023 All rights reversed by Reverse-Alpha Studios


#include "MMC_MaxHealth.h"
#include "QuantumWarfare/Ability/AttributeSystem.h"
#include <QuantumWarfare/Interaction/CombatInterface.h>

UMMC_MaxHealth::UMMC_MaxHealth()
{
	EnduranceDef.AttributeToCapture = UAttributeSystem::GetEnduranceAttribute();
	EnduranceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EnduranceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(EnduranceDef);

	StrengthDef.AttributeToCapture = UAttributeSystem::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(StrengthDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Endurance = 0.0f;
	GetCapturedAttributeMagnitude(EnduranceDef, Spec, EvaluationParameters, Endurance);
	Endurance		= FMath::Max<float>(Endurance, 0.0f);

	float Strength = 0.0f;
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParameters, Strength);
	Strength	   = FMath::Max<float>(Strength, 0.0f);

	ICombatInterface* CombatInterface	= Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel				= CombatInterface->GetPlayerLevel();

	return ((Strength + Endurance) * 2.0f) + (2.0f * PlayerLevel);
}
