// @2023 All rights reversed by Reverse-Alpha Studios


#include "HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBar::NativeConstruct()
{
}

void UHealthBar::SetBarValuePercent(float const Value)
{
	HealthProgressBar->SetPercent(Value);
}
