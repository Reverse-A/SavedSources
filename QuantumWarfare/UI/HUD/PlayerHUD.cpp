// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerHUD.h"
#include <QuantumWarfare/UI/Widget/PlayerOverlay.h>
#include <QuantumWarfare/UI/WidgetController/AttributeMenuWidgetController.h>
#include <QuantumWarfare/UI/WidgetController/PlayerOverlayWidgetController.h>

UPlayerOverlayWidgetController* APlayerHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
    if (OverlayWidgetController == nullptr)
    {
        OverlayWidgetController = NewObject<UPlayerOverlayWidgetController>(this, OverlayWidgetControllerClass);
        OverlayWidgetController->SetWidgetControllerParamaters(WCParams);
        OverlayWidgetController->BindCallbacksDependencies();

        return OverlayWidgetController;
    }
    return OverlayWidgetController;
}

UAttributeMenuWidgetController* APlayerHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
    if (AttributeMenuWidgetController == nullptr)
    {
        AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);

        AttributeMenuWidgetController->SetWidgetControllerParamaters(WidgetControllerParams);
        AttributeMenuWidgetController->BindCallbacksDependencies();

        return AttributeMenuWidgetController;
    }
    return AttributeMenuWidgetController;
}

void APlayerHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
    checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_PlayerHUD"));
    checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class Un-initialized, please fill out BP_PlayerHUD"));

    UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
    OverlayWidget = Cast<UPlayerUserWidget>(Widget);

    const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
    UPlayerOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

    OverlayWidget->SetWidgetController(WidgetController);
    WidgetController->BroadcastInitialValues();
    Widget->AddToViewport();
}
