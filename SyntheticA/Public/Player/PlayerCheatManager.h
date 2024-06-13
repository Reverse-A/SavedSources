// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "GameFramework/CheatManager.h"
#include "PlayerCheatManager.generated.h"

/**
 * 
 */

#ifndef USING_CHEAT_MANAGER
#define USING_CHEAT_MANAGER ( 1 && !UE_BUILD_SHIPPING)
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCheat, Log, All);

UCLASS(config = Game, Within = PlayerController)
class SYNTHETICA_API UPlayerCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	UPlayerCheatManager();

	virtual void InitCheatManager() override;

	static void CheatOutputText(const FString& TextToOutput);

	UFUNCTION(exec)
	void Cheat(const FString& Message);
};
