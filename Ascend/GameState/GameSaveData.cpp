// @2023 All rights reversed by Reverse-Alpha Studios


#include "GameSaveData.h"
#include "Kismet/GameplayStatics.h"

UGameSaveData::UGameSaveData()
{
}

bool UGameSaveData::SaveGameData(UGameSaveData* SaveData)
{
	if (!SaveData)
	{
		UE_LOG(LogTemp, Error, TEXT("SaveData is nullptr"));
		return false;
	}

	return UGameplayStatics::SaveGameToSlot(SaveData, TEXT("MySaveSlot"), 0);
}

UGameSaveData* UGameSaveData::LoadGameData()
{
	return Cast<UGameSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("MySaveSlot"), 0));
}
