// @2023 All rights reversed by Reverse-Alpha Studios


#include "MainMenuMode.h"
#include "QuantumWarfare/UI/Widget/MainMenu/MainMenu.h"
#include "Blueprint/UserWidget.h"

void AMainMenuMode::BeginPlay()
{
    Super::BeginPlay();

    check(MainMenuWidgetClass);

    // Check if MainMenuWidgetClass is valid
    if (MainMenuWidgetClass)
    {
        // Spawn the UMainMenu widget
        UMainMenu* MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuWidgetClass);

        // Check if the widget was successfully created
        if (MainMenuWidget)
        {
            // Add the widget to the viewport
            MainMenuWidget->AddToViewport();
        }
    }
}