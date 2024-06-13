// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSaveData.generated.h"


UCLASS()
class ASCEND_API UGameSaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	UGameSaveData();

    /** The player's transform */
    FTransform PlayerTransform;

    /** The player's level */
    int32 PlayerLevel;

    /** The player's experience points */
    float TotalExperiencePoints;

    /** The player's credits points */
    float TotalCredits;

    /** Save the game data to a save slot */
    UFUNCTION(BlueprintCallable, Category = "SaveData")
    static bool SaveGameData(UGameSaveData* SaveData);

    /** Load the game data from a save slot */
    UFUNCTION(BlueprintCallable, Category = "SaveData")
    static UGameSaveData* LoadGameData();

};
