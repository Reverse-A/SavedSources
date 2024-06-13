// @2023 All rights reversed by Reverse-Alpha Studios


#include "AttributeMenuWidgetController.h"
#include <QuantumWarfare/Ability/AttributeSystem.h>
#include <QuantumWarfare/Ability/Data/AttributeInfo.h>
#include <QuantumWarfare/QWGameplayTags.h>

void UAttributeMenuWidgetController::BindCallbacksDependencies()
{
	UAttributeSystem* AttributeSystem = CastChecked<UAttributeSystem>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AttributeSystem->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda
		(	
			[this, Pair] (const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAttributeSystem* PlayerAttributeSystem = CastChecked<UAttributeSystem>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : PlayerAttributeSystem->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FQWAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
