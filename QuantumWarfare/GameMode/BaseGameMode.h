// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

class UCharacterClassInformation;
UCLASS()
class QUANTUMWARFARE_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInformation> CharacterClassInformation;
};
