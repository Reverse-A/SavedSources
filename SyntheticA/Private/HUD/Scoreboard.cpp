// @2023 All rights reversed by Reverse-Alpha Studios


#include "HUD/Scoreboard.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "GameState/GameStateManager.h"

#pragma region GeneleraFunctions

void UScoreboard::NativeConstruct()
{
	Super::NativeConstruct();
	GameStateManager = Cast<AGameStateManager>(UGameplayStatics::GetGameState(this));
}

void UScoreboard::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("GenesisScore: %f"), GetGenesisPactScore());
	UE_LOG(LogTemp, Warning, TEXT("LibertyScore: %f"), GetVanguardSovereigntyScore());
}

void UScoreboard::UpdatePlayerName()
{

}
#pragma endregion

#pragma region Setters
void UScoreboard::SetPlayerTeam(const int32 PlayerIndex, const FString& TeamName)
{
	if (PlayerScoresArray.IsValidIndex(PlayerIndex))
	{
		PlayerScoresArray[PlayerIndex].PlayerTeam = TeamName;
	}
}

void UScoreboard::SetPlayerName(const int32 PlayerIndex, const FString& PlayerName)
{
	if (PlayerScoresArray.IsValidIndex(PlayerIndex))
	{
		PlayerScoresArray[PlayerIndex].PlayerName = PlayerName;
	}
}

void UScoreboard::SetPlayerKills(const int32 PlayerIndex, int32 Kills)
{
	if (PlayerScoresArray.IsValidIndex(PlayerIndex))
	{
		PlayerScoresArray[PlayerIndex].PlayerKills = Kills;
	}
}

void UScoreboard::SetPlayerAssists(const int32 PlayerIndex, uint32 Assists)
{
	if (PlayerScoresArray.IsValidIndex(PlayerIndex))
	{
		PlayerScoresArray[PlayerIndex].PlayerAsissts = Assists;
	}
}

void UScoreboard::SetPlayerDeaths(const int32 PlayerIndex, uint32 Deaths)
{
	if (PlayerScoresArray.IsValidIndex(PlayerIndex))
	{
		PlayerScoresArray[PlayerIndex].PlayerDeaths = Deaths;
	}
}

void UScoreboard::SetPlayerScore(const int32 PlayerIndex, uint32 Score)
{
	if (PlayerScoresArray.IsValidIndex(PlayerIndex))
	{
		PlayerScoresArray[PlayerIndex].PlayerScore = Score;
	}
}
#pragma endregion

#pragma region Getters
FString UScoreboard::GetPlayerTeam() const
{

	return FString();
}

FString UScoreboard::GetPlayerName() const
{
	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), PlayerActors);

	for (AActor* PlayerActor : PlayerActors)
	{
		APlayerController* PlayerController = Cast<APlayerController>(PlayerActor);
		if (PlayerController)
		{
			APlayerState* PlayerState = PlayerController->PlayerState;

			if (PlayerState)
			{
				FString PlayerName = PlayerState->GetPlayerName();
				//SetPlayerName(PlayerName);
				return PlayerName;
			}
		}
	}
	return FString();
}
#pragma endregion

#pragma region Teams
float UScoreboard::GetGenesisPactScore()
{
	GameStateManager = GameStateManager == nullptr ? Cast<AGameStateManager>(UGameplayStatics::GetGameState(this)) : GameStateManager;

	if (GameStateManager)
	{
		return GameStateManager->GenesisPactScore;
	}
	return 0.0f;
}

float UScoreboard::GetVanguardSovereigntyScore()
{
	GameStateManager = GameStateManager == nullptr ? Cast<AGameStateManager>(UGameplayStatics::GetGameState(this)) : GameStateManager;

	if (GameStateManager)
	{
		return GameStateManager->VanguardSovereigntyScore;
	}
	return 0.0f;
}
#pragma endregion