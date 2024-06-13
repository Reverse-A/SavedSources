// @2023 All rights reversed by Reverse-Alpha Studios


#include "Player/PlayerCheatManager.h"
#include "GameFramework/Pawn.h"
#include "PlayerController/PlayerControllerManager.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Console.h"
#include "GameFramework/HUD.h"
 

#include UE_INLINE_GENERATED_CPP_BY_NAME(PlayerCheatManager)

DEFINE_LOG_CATEGORY(LogPlayerCheat);

namespace PlayerCheat
{
	static const FName NAME_Fixed = FName(TEXT("Fixed"));

	static bool bEnableDebugCameraCycling = false;
	static FAutoConsoleVariableRef CVarEnableDebugCameraCycling
	(
		TEXT("PlayerCheat.EnableDebugCameraCycling"),
		bEnableDebugCameraCycling,
		TEXT("If true then you can cycle the debug camera while running the game."),
		ECVF_Cheat
	);

	static bool bStartInGodMode = false;
	static FAutoConsoleVariableRef CVarStartInGodMode
	(
		TEXT("PlayerCheat.StartInGodMode"),
		bStartInGodMode,
		TEXT("If true then the God cheat will be applied on begin play"),
		ECVF_Cheat
	);
}

UPlayerCheatManager::UPlayerCheatManager()
{
	//DebugCameraControllerClass = ASyntheticaDebugCameraController::StaticClass();
}

void UPlayerCheatManager::InitCheatManager()
{
	Super::InitCheatManager();

//#if WITH_EDITOR
//	if (GIsEditor)
//	{
//		APlayerController* PlayerController = GetOuterAPlayerController();
//		for (const FSyntheticaCheatToRun& CheatRow : GetDefault<USyntheticaDeveloperSettings>()->CheatsToRun)
//		{
//			if (CheatRow.Phase == ECheatExecutionTime::OnCheatManagerCreated)
//			{
//				PlayerController->ConsoleCommand(CheatRow.Cheat, /*bWriteToLog*/ true);
//			}
//		}
//	}
//#endif
//
//
//	if (SyntheticaCheat::bStartInGodMode)
//	{
//		God();
//	}
}

void UPlayerCheatManager::CheatOutputText(const FString& TextToOutput)
{
#if USING_CHEAT_MANAGER
	// Output to the console. 
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->ViewportConsole)
	{
		GEngine->GameViewport->ViewportConsole->OutputText(TextToOutput);
	}

	//

#endif
}

void UPlayerCheatManager::Cheat(const FString& Message)
{

}