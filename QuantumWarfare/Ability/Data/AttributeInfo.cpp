// @2023 All rights reversed by Reverse-Alpha Studios


#include "AttributeInfo.h"

FQWAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FQWAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT
		(
			"Can't find info for AttributeTag{%s} on Attribute Info [%s]."), 
			*AttributeTag.ToString(),
			*GetNameSafe(this)
		);			
	}

	return FQWAttributeInfo();
}
