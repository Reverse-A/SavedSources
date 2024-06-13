// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "PlayerUserWidget.h"
#include "PlayerOverlay.generated.h"

class UCanvasPanel;
class UProgressBar;
class UTextBlock;

UCLASS()
class QUANTUMWARFARE_API UPlayerOverlay : public UPlayerUserWidget
{
	GENERATED_BODY()
	
private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> CanvasPanel; 


/*Progress Bars*/
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HealthProgressBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> ShieldProgressBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> EnergyProgressBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> StaminaProgressBar;

/*Texts*/
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HealthTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ShieldTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> EnergyTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> StaminaTextBlock;

public:

};
