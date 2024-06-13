// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "QWAbilitySystemGlobals.generated.h"


UCLASS()
class QUANTUMWARFARE_API UQWAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
