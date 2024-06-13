// @2023 All rights reversed by Reverse-Alpha Studios


#include "MMC_MaxEnergy.h"

#include "QuantumWarfare/Ability/AttributeSystem.h"
#include <QuantumWarfare/Interaction/CombatInterface.h>

UMMC_MaxEnergy::UMMC_MaxEnergy()
{
	EnduranceDef.AttributeToCapture = UAttributeSystem::GetEnduranceAttribute();
	EnduranceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EnduranceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(EnduranceDef);

	IntelligenceDef.AttributeToCapture = UAttributeSystem::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxEnergy::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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

	float Intelligence = 0.0f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.0f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();



	/*Calculation Method*/
	// EnduranceInitialize = 10.0f;
	// 50 + 2 * 10 + 10.0f * 1; 
	return ((Intelligence + Endurance) * 2.0f) + (2.0f * PlayerLevel);
}