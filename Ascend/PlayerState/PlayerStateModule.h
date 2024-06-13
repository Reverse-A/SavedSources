// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateModule.generated.h"

struct LevelRequirements
{
	float RequiredExperience;
};

class ABaseCharacter;
class AAIEntityModule;

UCLASS()
class ASCEND_API APlayerStateModule : public APlayerState
{
	GENERATED_BODY()
	
public:
	APlayerStateModule();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

};
