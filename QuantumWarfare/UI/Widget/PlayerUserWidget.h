// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUserWidget.generated.h"

class USizeBox;
class UOverlay;
class UImage;
class UTextBlock;
class UTexture2D;
class UProgressBar;

struct FSlateBrush;

UCLASS()
class QUANTUMWARFARE_API UPlayerUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

private:
#pragma region Health And Shield
	/*Health and Shield*/
	UOverlay* HealthShieldOverlay;

	UImage* HealthShieldBackground;

	UProgressBar* HealthProgressBar;

	UTextBlock* HealthTextBlock;

	UProgressBar* ShieldProgressBar;

	UTextBlock* ShieldTextBlock;

	UTextBlock* HealthShieldDivider;
#pragma endregion

#pragma region Energy And Stamina
	UOverlay* EnergyStaminaOverlay;

	UImage* EnergyStaminaBackground;

	UProgressBar* EnergyProgressBar;

	UTextBlock* EnergyTextBlock;

	UProgressBar* StaminaProgressBar;

	UTextBlock* StaminaTextBlock;

	UTextBlock* EnergyStaminaDivider;

#pragma region
};





























//public:
//	virtual void NativePreConstruct() override;
//
//	UPROPERTY(BlueprintReadOnly)
//	TObjectPtr<UObject> WidgetController;
//
//	UFUNCTION(BlueprintCallable)
//	void SetWidgetController(UObject* InWidgetController);
//
//protected:
//	UFUNCTION(BlueprintImplementableEvent)
//	void WidgetControllerSet();
//
///*SizeBox*/
//protected:
//	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
//	USizeBox* SizeBoxRoot;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	float BoxWidth = 150.0f;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	float BoxHeight = 10.0f;
//
///*Overlay*/
//	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
//	UOverlay* OverlayRoot;
//
///*Background*/
//	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
//	UImage* BackgroundImage;
//
//	UPROPERTY(EditAnywhere)
//	FSlateBrush BackgroundImageSlateBrush;
//
///*ProgressBar*/
//	UPROPERTY(meta = (BindWidget))
//	UProgressBar* ProgressBar;
//
//	UPROPERTY(EditAnywhere)
//	FLinearColor ProgressBarFillColor;
//
//	UPROPERTY(BlueprintReadOnly, EditAnywhere)
//	FSlateBrush ProgressBarSlateBrush;
//
//	/*Pre-Construct | Functions*/
//private:
//	void UpdateSizeBox(USizeBox* InSizeBox, float InWidth, float InHeight);
//	void UpdateBackgroundBrush();
//	void UpdateProgressBarBrush();