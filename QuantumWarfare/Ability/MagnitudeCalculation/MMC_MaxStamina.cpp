// @2023 All rights reversed by Reverse-Alpha Studios


#include "MMC_MaxStamina.h"

#include "QuantumWarfare/Ability/AttributeSystem.h"
#include <QuantumWarfare/Interaction/CombatInterface.h>

UMMC_MaxStamina::UMMC_MaxStamina()
{
	EnduranceDef.AttributeToCapture = UAttributeSystem::GetEnduranceAttribute();
	EnduranceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EnduranceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(EnduranceDef);

	AgilityDef.AttributeToCapture = UAttributeSystem::GetAgilityAttribute();
	AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AgilityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AgilityDef);
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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

	float Stamina = 0.0f;
	GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluationParameters, Stamina);
	Stamina = FMath::Max<float>(Stamina, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();



	return ((Stamina + Endurance) * 2.0f) + (2.0f * PlayerLevel);
}