#pragma once

UENUM(BlueprintType)
enum class ETeam : uint8
{
	ET_VanguardSovereignty	UMETA	(DisplayName = "VanguardSovereignty"),
	ET_GenesisPact			UMETA	(DisplayName = "GenesisPact"),
	ET_NoTeam				UMETA	(DisplayName = "NoTeam"),

	ET_MAX					UMETA	(DisplayName = "DefaultMax")
};
