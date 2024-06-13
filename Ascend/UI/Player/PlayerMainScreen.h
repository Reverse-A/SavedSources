// // @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainScreen.generated.h"

class UProgressBar;
class UTextBlock;
UCLASS()
class ASCEND_API UPlayerMainScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ShieldProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ExperienceProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ExperienceAndLevelText;
};
