// ©2023 Reverse-A. All rights reserved.

#include "HealthBarComponent.h"
#include "Synthetica/HUD/HealthBar.h"
#include "Synthetica/Weapons/Projectile.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}

}
