// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "PlayerInputConfig.h"
#include "PlayerInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class QUANTUMWARFARE_API UPlayerInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>

	void BindAbilityActions
	(
		const UPlayerInputConfig* InputConfig,
		UserClass* Object, 
		PressedFuncType PressedFunc,
		ReleasedFuncType ReleasedFunc, 
		HeldFuncType HeldFunc
	);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UPlayerInputComponent::BindAbilityActions(const UPlayerInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FPlayerInputAction& Action : InputConfig->AbilityInputAction)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}