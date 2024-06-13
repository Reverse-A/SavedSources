// // @2023 All rights reversed by Reverse-Alpha Studios


#include "ViewInterfaceComponent.h"

#include "Ascend/Character/BaseCharacter.h"

#include "Ascend/Component/ArsenalComponent.h"

UViewInterfaceComponent::UViewInterfaceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UViewInterfaceComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UViewInterfaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//DrawCrosshairToViewPort(DeltaTime);
}

void UViewInterfaceComponent::DrawCrosshairToViewPort(float DeltaSeconds)
{
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetOwner()) : BaseCharacter;
	OwnerPlayerController = OwnerPlayerController == nullptr ? Cast<APlayerController>(BaseCharacter->Controller) : OwnerPlayerController;

	if (OwnerPlayerController)
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(OwnerPlayerController->GetHUD()) : PlayerHUD;

		if (PlayerHUD)
		{
			if (BaseCharacter)
			{
				if (BaseCharacter->GetArsenalSystem()->GetPrimaryEquipWeapon() == nullptr)
				{
					PlayerHUD->GetCrosshairPackage().SetCrosshairCenter	(CrosshairCenter);
					PlayerHUD->GetCrosshairPackage().SetCrosshairRight	(nullptr);
					PlayerHUD->GetCrosshairPackage().SetCrosshairLeft	(nullptr);
					PlayerHUD->GetCrosshairPackage().SetCrosshairTop	(nullptr);
					PlayerHUD->GetCrosshairPackage().SetCrosshairBottom	(nullptr);
				}
				else if (BaseCharacter->GetArsenalSystem()->GetPrimaryEquipWeapon() != nullptr)
				{
					if (BaseCharacter->GetArsenalSystem()->GetPrimaryEquipWeapon() == nullptr) return;

					PlayerHUD->GetCrosshairPackage().SetCrosshairCenter	(CrosshairCenter);
					PlayerHUD->GetCrosshairPackage().SetCrosshairRight	(CrosshairRight);
					PlayerHUD->GetCrosshairPackage().SetCrosshairLeft	(CrosshairLeft);
					PlayerHUD->GetCrosshairPackage().SetCrosshairTop	(CrosshairTop);
					PlayerHUD->GetCrosshairPackage().SetCrosshairBottom	(CrosshairBottom);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BaseCharacter == nullptr"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerHUD == nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPlayerController == nullptr"));
	}
}

