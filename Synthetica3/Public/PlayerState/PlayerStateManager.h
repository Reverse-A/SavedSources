// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateManager.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API APlayerStateManager : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
	* Replication Notifies
	*/
	virtual void OnRep_Score() override;

	UFUNCTION()
	virtual void OnRep_Defeats();

	virtual void AddToScore(float ScoreAmount);
	virtual void AddToDefeats(int DefeatsAmount);


private:
	UPROPERTY()
	class ABaseCharacter*			BaseCharacter;

	UPROPERTY()
	class APlayerControllerManager* PlayerControllerManager;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;
};
