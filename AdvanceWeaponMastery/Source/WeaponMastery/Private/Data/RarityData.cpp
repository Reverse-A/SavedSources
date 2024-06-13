#include "Data/RarityData.h"

ERarityEffectProcess URarityData::GetRarityEffectProcess() const
{
	return RarityEffectProcess;
}

void URarityData::SetRarityEffectProcess(ERarityEffectProcess InRarityEffectProcess)
{
	RarityEffectProcess = InRarityEffectProcess;
}

// Inline function implementation to retrieve the current rarity type.
 // This function returns the value of the RarityType property.
ERarityType URarityData::GetRarityType() const
{
	return RarityType;
}

// Function implementation to set a new rarity type for this instance.
// This function assigns the provided value (InRarityType) to the RarityType property.
void URarityData::SetRarityType(ERarityType InRarityType)
{
	RarityType = InRarityType;
}