// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Scoreboard.generated.h"


/*"We have two teams, one is called the 'Genesis Pact' and the other is the 'Vanguard Sovereignty'.*/
USTRUCT(BlueprintType)
struct FPlayerScores
{
	GENERATED_BODY()

	UPROPERTY()
	FString PlayerTeam;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 PlayerKills;

	UPROPERTY()
	uint32 PlayerAsissts;


	UPROPERTY()
	uint32 PlayerDeaths;


	UPROPERTY()
	uint32 PlayerScore;

	FPlayerScores()
		: PlayerTeam(TEXT(""))
		, PlayerName(TEXT(""))
		, PlayerKills(0)
		, PlayerAsissts(0)
		, PlayerDeaths(0)
		, PlayerScore(0)
	{

	}
};

UCLASS()
class SYNTHETICA_API UScoreboard : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
    TArray<FPlayerScores> PlayerScoresArray;

    UPROPERTY()
    class AGameStateManager* GameStateManager;
protected:
    void UpdatePlayerName();

public:
    // Functions to set and get player scores
    void SetPlayerTeam(const int32 PlayerIndex, const FString& TeamName);
    void SetPlayerName(const int32 PlayerIndex, const FString& PlayerName);
    void SetPlayerKills(const int32 PlayerIndex, int32 Kills);
    void SetPlayerAssists(const int32 PlayerIndex, uint32 Assists);
    void SetPlayerDeaths(const int32 PlayerIndex, uint32 Deaths);
    void SetPlayerScore(const int32 PlayerIndex, uint32 Score);

    FString GetPlayerTeam() const;
    FString GetPlayerName() const;


/*TeamScores*/
private:
    UPROPERTY()
    float GenesisPactScore;

    UPROPERTY()
    float VanguardSovereigntyScore;

    float GetGenesisPactScore();
    float GetVanguardSovereigntyScore();
};
