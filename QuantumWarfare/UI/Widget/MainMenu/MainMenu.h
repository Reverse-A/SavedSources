// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class AMainMenuMode;

class UButton;

UCLASS()
class QUANTUMWARFARE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	// Function to access MainMenuWidgetClass
	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	TSubclassOf<class UMainMenu> GetMainMenuWidgetClass();

private:
	AMainMenuMode* MainMenuWidgetClass;


private:
	// Declare a smart pointer to a UButton object named StartButton.
	// This button is intended to be bound to a widget in the user interface.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	// Declare a smart pointer to a UButton object named OptionsButton.
	// This button is intended to be bound to a widget in the user interface.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionsButton;

	// Declare a smart pointer to a UButton object named CreditsButton.
	// This button is intended to be bound to a widget in the user interface.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreditsButton;

	// Declare a smart pointer to a UButton object named QuitButton.
	// This button is intended to be bound to a widget in the user interface.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
};
