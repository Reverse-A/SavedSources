// ©2023 Reverse - A Company. All rights reserved.


#include "PlayerState/PlayerStateManager.h"

#include "Character/BaseCharacter.h"
#include "PlayerController/PlayerControllerManager.h"


#include "Net/UnrealNetwork.h"

void APlayerStateManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerStateManager, Defeats);
}

void APlayerStateManager::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);

	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDScore(GetScore());
		}
	}
}

void APlayerStateManager::OnRep_Score()
{
	Super::OnRep_Score();


	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDScore(GetScore());
		}
	}
}

void APlayerStateManager::AddToDefeats(int DefeatsAmount)
{
	Defeats += DefeatsAmount;

	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDDefeats(Defeats);
		}
	}
}

void APlayerStateManager::OnRep_Defeats()
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetPawn()) : BaseCharacter;
	if (BaseCharacter)
	{
		PlayerControllerManager = PlayerControllerManager == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerControllerManager;

		if (PlayerControllerManager)
		{
			PlayerControllerManager->SetHUDDefeats(Defeats);
		}
	}
}

