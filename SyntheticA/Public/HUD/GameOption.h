// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOption.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SYNTHETICA_API UGameOption : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void MenuSetup();
	void MenuTearDown();

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

	UFUNCTION()
	void OnPlayerLeftGame();

#pragma region Class&Components
private:
	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UPROPERTY()
	class APlayerController* PlayerController;

	UPROPERTY()
	class UGameUserSettings* GameUserSettings;
#pragma endregion

private:
	UPROPERTY(meta = (BindWidget)) UButton* ApplyButton;
	UPROPERTY(meta = (BindWidget)) UButton* BackButton;
	UPROPERTY(meta = (BindWidget)) UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget)) UButton* WindowModeIncrease;
	UPROPERTY(meta = (BindWidget)) UButton* WindowModeDecrease;

	UPROPERTY(meta = (BindWidget)) UButton* ResolutionIncrease;
	UPROPERTY(meta = (BindWidget)) UButton* ResolutionDecrease;

	UPROPERTY(meta = (BindWidget)) UButton* GraphicsIncrease;
	UPROPERTY(meta = (BindWidget)) UButton* GraphicsDecrease;

	UPROPERTY(meta = (BindWidget)) UButton* VsyncIncrease;
	UPROPERTY(meta = (BindWidget)) UButton* VsyncDecrease;

	UPROPERTY(meta = (BindWidget)) UButton* FrameRateIncrease;
	UPROPERTY(meta = (BindWidget)) UButton* FrameRateDecrease;

	UPROPERTY(meta = (BindWidget)) UTextBlock* WindowModeText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ResolutionText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* GraphicsText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* VsyncText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* FrameText;



private:
	UFUNCTION()
	void ApplyButtonClicked();

	UFUNCTION()
	void BackButtonClicked();

	UFUNCTION()	
	void QuitGameButtonClicked();


#pragma region Windows Mode
	EWindowMode::Type WindowMode;

	uint8 WindowModeIndex;

	UFUNCTION()
	void WindowModeIncreaseButtonClicked();

	UFUNCTION()
	void WindowModeDecreaseButtonClicked();
#pragma endregion

#pragma region Resolution
	uint8 ResolutionIndex;

	FIntPoint Resolution;
	int ResolutionX;
	int ResolutionY;

	UFUNCTION()
	void ResolutionIncreaseButtonClicked();

	UFUNCTION()
	void ResolutionDecreaseButtonClicked();


	void Select_ResolutionScaleByIndex();
#pragma endregion
#pragma region Graphics
	uint8 GraphicsIndex;

	UFUNCTION()
	void GraphicsIncreaseButtonClicked();

	UFUNCTION()
	void GraphicsDecreaseButtonClicked();

	void SetGraphicsText(FString& outString);
#pragma endregion
#pragma region V-Sync
	bool bVsyncOn;

	UFUNCTION()
	void VSyncOnButtonClicked();

	UFUNCTION()
	void VSyncOffButtonClicked();
#pragma endregion
#pragma region FrameRate

	uint32 FrameRate;

	UFUNCTION()
	void FrameRateIncreaseButtonClicked();

	UFUNCTION()
	void FrameRateDecreaseButtonClicked();
#pragma endregion
};
