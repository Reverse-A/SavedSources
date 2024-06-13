// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxShield.generated.h"

/**
 * 
 */
UCLASS()
class QUANTUMWARFARE_API UMMC_MaxShield : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxShield();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition EnduranceDef;
	FGameplayEffectAttributeCaptureDefinition DexterityDef;
};
