// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelSystemComponent.generated.h"

class ABaseCharacter;

USTRUCT(BlueprintType)
struct FLevelSystemStructure
{
	GENERATED_BODY()

protected:
	// Player's level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerLevel;

	// Total accumulated experience points
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TotalExperience;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AIEntityLevel;

public:
	// Default constructor initializing PlayerLevel to 1 and TotalExperience to 0.0
	FLevelSystemStructure()		
	:	PlayerLevel(1),				// Initial player level set to 1.
		TotalExperience(0.0f),		// Initial total experience set to 0.0
		AIEntityLevel(PlayerLevel)	// AIEntityLevel initialized to the same value as PlayerLevel for consistency.
	{

	}

public:
	// Getter for PlayerLevel
	int32 GetPlayerLevel() const
	{
		return PlayerLevel;
	}

	void SetPlayerLevel(int32 Level)
	{
		PlayerLevel = Level;
	}

	int32 GetAILevel() const
	{
		return AIEntityLevel;
	}

	void SetAIEntityLevel(int32 Level)
	{
		AIEntityLevel = Level;
	}

	float GetTotalExperience() const
	{
		return TotalExperience;
	}

	void SetTotalExperience(float TotalExp)
	{
		TotalExperience = TotalExp;
	}

	float GetAndCalculateExperiencePoint()
	{
		if (AIEntityLevel >= 0 && AIEntityLevel <= 50)
		{
			//float DefaultRandomExperience = FMath::FRandRange(20.0f, 35.0f);
			float DefaultRandomExperience = FMath::FRandRange(200.0f, 500.0f);

			float ExperienceBonus = DefaultRandomExperience * AIEntityLevel;
			return  ExperienceBonus;
		}
		return 0.0f;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCEND_API ULevelSystemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
private:
#pragma region BaseCharacter
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

public:
	ABaseCharacter* GetBaseCharacter() const { return BaseCharacter; }

	void SetBaseCharacter(ABaseCharacter* InCharacter)
	{
		BaseCharacter = InCharacter;
	}
#pragma endregion
private:
	FLevelSystemStructure LevelSystemStructure;

public:
	FLevelSystemStructure GetLevelStructure() const
	{ 
		return LevelSystemStructure;
	}

public:
	// Function to increase total experience by a specified amount
	void IncreaseExperience(float Experience);
	float GetCurrentLevelThreshold() const;

protected:
	void UpdateLevel(float IncreaseExperienceAmount);

#pragma region Save&Load

public:
	void SaveExperienceAndLevel(uint32 Level, float TotalExperience);

#pragma endregion
};
