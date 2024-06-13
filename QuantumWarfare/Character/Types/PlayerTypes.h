// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EQWPlayerState : uint8
{
    Alive,              // No combat mode
    Neutral,            // Player is idle, not engaged in combat
    Engaged,            // Player is actively engaged in combat
    Defending,          // Player is defending or guarding
    Attacking,          // Player is attacking
    Injured,            // Player is injured
    Dead,               // Player is dead
};

UENUM(BlueprintType)
enum class EEquipmentState : uint8
{
    Unequipped,   // Weapon is not currently equipped
    Equipped,     // Weapon is currently equipped
};

