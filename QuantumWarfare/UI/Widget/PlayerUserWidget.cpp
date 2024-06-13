// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerUserWidget.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include <Blueprint/WidgetLayoutLibrary.h>


void UPlayerUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UPlayerUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}



















































//void UPlayerUserWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//
//	UpdateSizeBox(SizeBoxRoot, BoxWidth, BoxHeight);
//	UpdateBackgroundBrush();
//	UpdateProgressBarBrush();
//}
//
//void UPlayerUserWidget::SetWidgetController(UObject* InWidgetController)
//{
//	WidgetController = InWidgetController;
//	WidgetControllerSet();
//}
//
//
//void UPlayerUserWidget::UpdateSizeBox(USizeBox* InSizeBox, float InWidth, float InHeight)
//{
//	SizeBoxRoot->SetWidthOverride(InWidth);
//	SizeBoxRoot->SetHeightOverride(InHeight);
//}
//
//void UPlayerUserWidget::UpdateBackgroundBrush()
//{
//	BackgroundImage->SetBrush(BackgroundImageSlateBrush);
//}
//
//void UPlayerUserWidget::UpdateProgressBarBrush()
//{
//	ProgressBar->SetFillColorAndOpacity(ProgressBarFillColor);
//}

