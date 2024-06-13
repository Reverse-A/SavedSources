#pragma once

UENUM(BlueprintType)
enum class ECameraViewPerspective : uint8
{
	ECVP_None	            UMETA(Hidden),                             // Hidden enumerator.
	ECVP_FirstPerson        UMETA(DisplayName = "FirstPersonCamera"),  // Represents the first-person camera view perspective.
	ECVP_ThirdPerson        UMETA(DisplayName = "ThirdPersonCamera"),  // Represents the third-person camera view perspective.

	ECVP_Custom_1           UMETA(DisplayName = "Reserved0"),          // Custom enumerator 0 (Reserved for future usage).
	ECVP_Custom_2           UMETA(DisplayName = "Reserved1"),          // Custom enumerator 1 (Reserved for future usage).
	ECVP_Custom_3           UMETA(DisplayName = "Reserved2"),          // Custom enumerator 2 (Reserved for future usage).
	ECVP_Custom_4           UMETA(DisplayName = "Reserved3"),          // Custom enumerator 3 (Reserved for future usage).
	ECVP_MAX                UMETA(Hidden),                             // Hidden enumerator used to indicate the maximum value.
};
