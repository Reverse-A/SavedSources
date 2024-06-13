#pragma once

UENUM(BlueprintType)
enum class ETroopTypes : uint8
{
	ETT_Initial				UMETA(DisplayName = "Initial"),
	ETT_Infantry			UMETA(DisplayName = "Infantry"),
	ETT_AerialPilot			UMETA(DisplayName = "AerialPilot"),
	ETT_ArmoredCommander	UMETA(DisplayName = "ArmoredCommander"),
	ETT_TransporterDriver	UMETA(DisplayName = "TransporterDriver"),

	ET_MAX					UMETA(DisplayName = "DefaultMax")
};