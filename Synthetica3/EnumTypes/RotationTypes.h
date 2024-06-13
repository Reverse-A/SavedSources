#pragma once

#include "RotationTypes.generated.h"


UENUM(BlueprintType)
enum class ECharacterRotation : uint8
{
	ECR_Left	UMETA(DisplayName = "Turning Left"),
	ECR_Right	UMETA(DisplayName = "Turning Right"),
	ECR_None	UMETA(DisplayName = "Not Turning"),

	ECR_MAX		UMETA(DisplayName = "MAX")
};