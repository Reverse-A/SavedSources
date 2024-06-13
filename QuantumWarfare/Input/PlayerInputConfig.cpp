// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerInputConfig.h"

const UInputAction* UPlayerInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FPlayerInputAction& Action : AbilityInputAction)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT
		(
			"Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"),
			*InputTag.ToString(),
			*GetNameSafe(this)
		);
	}

	return nullptr;
}
