// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerWidgetController.h"

void UPlayerWidgetController::SetWidgetControllerParamaters(const FWidgetControllerParams& InWidgetControllerParamaters)
{
	PlayerController = InWidgetControllerParamaters.PlayerController;
	PlayerState = InWidgetControllerParamaters.PlayerState;
	AbilitySystemComponent = InWidgetControllerParamaters.AbilitySystemComponent;
	AttributeSet = InWidgetControllerParamaters.AttributeSet;
}

void UPlayerWidgetController::BroadcastInitialValues()
{

}

void UPlayerWidgetController::BindCallbacksDependencies()
{

}
