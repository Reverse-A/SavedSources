// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"

#include "Characters/BaseCharacter.h"
#include "Components/CombatComponent.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	UGameInstance* GameInstance = GetGameInstance();

	if (GameInstance)
	{
		UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		check(Subsystem);

		if (NumberOfPlayers == Subsystem->Get_DesiredNumberOfPublicConnections())
		{
			UWorld* World = GetWorld();
			if (World)
			{
				bUseSeamlessTravel = true;
				FString MatchType = Subsystem->Get_DesiredMatchType();
				
				if (MatchType == "DeathMatch")
				{
					World->ServerTravel(FString("/Game/Maps/SyntheticaDeathMatch?listen"));
					UE_LOG(LogTemp, Warning, TEXT("DeathMatch"));
				}
				else if (MatchType == "TeamDeathMatch")
				{
					World->ServerTravel(FString("/Game/Maps/SyntheticaTeamDeathMatch?listen"));
					UE_LOG(LogTemp, Warning, TEXT("TeamDeathMatch"));
				}
				else if (MatchType == "CaptureTheFlagMatch")
				{
					World->ServerTravel(FString("/Game/Maps/SyntheticaCaptureTheFlagMatch?listen"));
					UE_LOG(LogTemp, Warning, TEXT("TeamDeathMatch"));
				}
			}
		}
	}
}


/*Garbage*/

//if (NumberOfPlayers == 2)
//{
//	UWorld* World = GetWorld();
//
//	if (World)
//	{
//		bUseSeamlessTravel = true;
//
//		World->ServerTravel(FString("/Game/Maps/SyntheticaDeathMatch?listen"));
//		LOG_WARNING("DONE")
//	}
//}