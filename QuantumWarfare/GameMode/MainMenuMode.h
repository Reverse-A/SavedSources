// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuMode.generated.h"

/**
 * 
 */
UCLASS()
class QUANTUMWARFARE_API AMainMenuMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainMenu> MainMenuWidgetClass;

public:
	// Getter function to access MainMenuWidgetClass
	TSubclassOf<class UMainMenu> GetMainMenuWidgetClass() const
	{
		return MainMenuWidgetClass;
	}

};
