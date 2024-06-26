// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDPanel.generated.h"

class UProgressBar;
class UTextBlock;
UCLASS()
class SYNTHETICA_API UPlayerHUDPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar*	HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock*		HealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*		ScoreAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*		DefeatsAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*		WeaponAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*		CarriedAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*		MatchCountDownText;
};
