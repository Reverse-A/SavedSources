// @2023 All rights reversed by Reverse-Alpha Studios


#include "MainMenu.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "QuantumWarfare/GameMode/MainMenuMode.h"
void UMainMenu::NativeConstruct()
{
    Super::NativeConstruct();
}

TSubclassOf<class UMainMenu> UMainMenu::GetMainMenuWidgetClass()
{
    // Get a reference to the game mode
    AMainMenuMode* MainMenuMode = Cast<AMainMenuMode>(GetWorld()->GetAuthGameMode());
    if (MainMenuMode)
    {
        // Call the getter function in the game mode to access MainMenuWidgetClass
        return MainMenuMode->GetMainMenuWidgetClass();
    }
    return nullptr;
}

