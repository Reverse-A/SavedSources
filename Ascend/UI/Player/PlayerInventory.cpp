// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerInventory.h"
#include "Ascend/Character/BaseCharacter.h"
#include "Ascend/Component/ArsenalComponent.h"
#include "Ascend/Weapon/BaseWeapon.h"

#include "Components/SizeBox.h"

void UPlayerInventory::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());

	if (PlayerCharacter)
	{
		ArsenalComponent = PlayerCharacter->FindComponentByClass<UArsenalComponent>();

		if (ArsenalComponent)
		{
			TArray<ABaseWeapon*> InventoryWeapons = ArsenalComponent->GetInventoryWeapons();

			ItemContainer->ClearChildren();

			for (auto InventoryWeapon : InventoryWeapons)
			{
				FWeaponInventoryStructure InventoryStructure = InventoryWeapon->GetWeaponInventoryStructure();

				UTexture2D* ItemImage = InventoryStructure.ItemImage;
				FString ItemDescription = InventoryStructure.ItemDescription;
				FName ItemName = InventoryStructure.ItemName;
				float ItemPrice = InventoryStructure.ItemPrice;

				UE_LOG(LogTemp, Warning, TEXT("ItemPrice: %f"), ItemPrice);
			}
			UE_LOG(LogTemp, Warning, TEXT("NativeConstruct"));
		}
	}
}
