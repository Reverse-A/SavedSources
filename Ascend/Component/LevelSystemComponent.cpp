// @2023 All rights reversed by Reverse-Alpha Studios


#include "LevelSystemComponent.h"

#include "Ascend/GameState/GameSaveData.h"

void ULevelSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULevelSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULevelSystemComponent::IncreaseExperience(float Experience)
{
	UpdateLevel(Experience);
}


void ULevelSystemComponent::UpdateLevel(float IncreaseExperienceAmount)
{	
	float CurrentTotalExperience = LevelSystemStructure.GetTotalExperience();

	LevelSystemStructure.SetTotalExperience(CurrentTotalExperience + IncreaseExperienceAmount);

	// Level thresholds for levels 1 to 50.
	// These constants represent the total experience thresholds required to achieve each level.
	// Adjustments to these thresholds may be necessary to balance gameplay progression.
	const TArray<float> LevelThresholds =
	{
		0.0f,     // Level 1
		500.0f,   // Level 2
		1000.0f,  // Level 3
		1500.0f,  // Level 4
		2000.0f,  // Level 5
		3000.0f,  // Level 6
		4000.0f,  // Level 7
		5000.0f,  // Level 8
		6000.0f,  // Level 9
		7000.0f,  // Level 10
		9000.0f,  // Level 11
		11000.0f, // Level 12
		13000.0f, // Level 13
		15000.0f, // Level 14
		17000.0f, // Level 15
		20000.0f, // Level 16
		23000.0f, // Level 17
		26000.0f, // Level 18
		29000.0f, // Level 19
		32000.0f, // Level 20
		35000.0f, // Level 21
		38000.0f, // Level 22
		41000.0f, // Level 23
		44000.0f, // Level 24
		47000.0f, // Level 25
		50000.0f, // Level 26
		53000.0f, // Level 27
		56000.0f, // Level 28
		59000.0f, // Level 29
		62000.0f, // Level 30
		65000.0f, // Level 31
		68000.0f, // Level 32
		71000.0f, // Level 33
		74000.0f, // Level 34
		77000.0f, // Level 35
		80000.0f, // Level 36
		83000.0f, // Level 37
		86000.0f, // Level 38
		89000.0f, // Level 39
		92000.0f, // Level 40
		95000.0f, // Level 41
		98000.0f, // Level 42
		101000.0f, // Level 43
		104000.0f, // Level 44
		107000.0f, // Level 45
		110000.0f, // Level 46
		113000.0f, // Level 47
		116000.0f, // Level 48
		119000.0f, // Level 49
		122000.0f  // Level 50
	};

	// Determine the player's level based on total experience
	if (LevelSystemStructure.GetTotalExperience() < 500.0f)
	{
		// If total experience is less than LevelOne threshold, set level to 1
		LevelSystemStructure.SetPlayerLevel(1);
		// Exit the function early
		return;
	}

	//Get the maximum level
	int32 MaxLevel = LevelThresholds.Num();
	// Determine the player's level based on total experience
	for (int i = 1; i <= MaxLevel; ++i) // Assume max level is 50
	{
		if (LevelSystemStructure.GetTotalExperience() < LevelThresholds[i])
		{
			LevelSystemStructure.SetPlayerLevel(i + 1);
			LevelSystemStructure.SetAIEntityLevel(i + 1);
			break;
		}
	}
}

void ULevelSystemComponent::SaveExperienceAndLevel(uint32 Level, float TotalExperience)
{
	UE_LOG(LogTemp, Log, TEXT("Inside SaveExperienceAndLevel"));

	UGameSaveData* SaveData = NewObject<UGameSaveData>();

	if (SaveData)
	{
		SaveData->PlayerLevel			= Level;
		SaveData->TotalExperiencePoints = TotalExperience;
		UE_LOG(LogTemp, Log, TEXT("Saving player level: %d, total experience: %f"), Level, TotalExperience);

		if (UGameSaveData::SaveGameData(SaveData))
		{
			UE_LOG(LogTemp, Log, TEXT("Game data saved successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to save game data"));
		}	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create UGameSaveData object"));
	}
}

float ULevelSystemComponent::GetCurrentLevelThreshold() const
{
	const TArray<float> LevelThresholds =
	{
		0.0f,     // Level 1
		500.0f,   // Level 2
		1000.0f,  // Level 3
		1500.0f,  // Level 4
		2000.0f,  // Level 5
		3000.0f,  // Level 6
		4000.0f,  // Level 7
		5000.0f,  // Level 8
		6000.0f,  // Level 9
		7000.0f,  // Level 10
		9000.0f,  // Level 11
		11000.0f, // Level 12
		13000.0f, // Level 13
		15000.0f, // Level 14
		17000.0f, // Level 15
		20000.0f, // Level 16
		23000.0f, // Level 17
		26000.0f, // Level 18
		29000.0f, // Level 19
		32000.0f, // Level 20
		35000.0f, // Level 21
		38000.0f, // Level 22
		41000.0f, // Level 23
		44000.0f, // Level 24
		47000.0f, // Level 25
		50000.0f, // Level 26
		53000.0f, // Level 27
		56000.0f, // Level 28
		59000.0f, // Level 29
		62000.0f, // Level 30
		65000.0f, // Level 31
		68000.0f, // Level 32
		71000.0f, // Level 33
		74000.0f, // Level 34
		77000.0f, // Level 35
		80000.0f, // Level 36
		83000.0f, // Level 37
		86000.0f, // Level 38
		89000.0f, // Level 39
		92000.0f, // Level 40
		95000.0f, // Level 41
		98000.0f, // Level 42
		101000.0f, // Level 43
		104000.0f, // Level 44
		107000.0f, // Level 45
		110000.0f, // Level 46
		113000.0f, // Level 47
		116000.0f, // Level 48
		119000.0f, // Level 49
		122000.0f  // Level 50
	};

	int32 CurrentPlayerLevel = LevelSystemStructure.GetPlayerLevel();

	if (CurrentPlayerLevel > 0 && CurrentPlayerLevel <= LevelThresholds.Num())
	{
		return LevelThresholds[CurrentPlayerLevel];
	}
	return 0.0f; // Default to 0.0f if level not found or invalid}

}
