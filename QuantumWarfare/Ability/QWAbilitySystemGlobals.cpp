// @2023 All rights reversed by Reverse-Alpha Studios


#include "QWAbilitySystemGlobals.h"

#include <QuantumWarfare/Ability/AbilitySystemTypes.h>

FGameplayEffectContext* UQWAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FQWGameplayEffectContext();
}