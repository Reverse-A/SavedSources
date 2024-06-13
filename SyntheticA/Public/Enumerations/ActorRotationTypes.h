#pragma once

UENUM(BlueprintType)
enum class ECharacterRotateDirection : uint8
{
	ECRD_None	UMETA(DisplayName = "NULL"),
	ECRD_Right	UMETA(DisplayName = "Right Direction"),
	ECRD_Left	UMETA(DisplayName = "Left Direction"),

	ECRD_MAX	UMETA(DisplayName = "Default MAX")
};