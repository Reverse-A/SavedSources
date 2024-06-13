// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UAttributeSet;
class UAttributeMenuWidgetController;
class UAbilitySystemComponent;
class UPlayerOverlayWidgetController;
class UPlayerUserWidget;

struct FWidgetControllerParams;
UCLASS()
class QUANTUMWARFARE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
#pragma region PlayerOverlay[Widget]
public:
    UPlayerOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

    UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);

    void InitOverlay
    (
        APlayerController* PC, 
        APlayerState* PS,
        UAbilitySystemComponent* ASC,
        UAttributeSet* AS
    );

private:
    UPROPERTY()
    TObjectPtr<UPlayerUserWidget> OverlayWidget;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UPlayerUserWidget> OverlayWidgetClass;

    UPROPERTY()
    TObjectPtr<UPlayerOverlayWidgetController> OverlayWidgetController;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UPlayerOverlayWidgetController> OverlayWidgetControllerClass;


/*Attribute Menu Widget Controller*/
    UPROPERTY()
    TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
#pragma endregion
};
