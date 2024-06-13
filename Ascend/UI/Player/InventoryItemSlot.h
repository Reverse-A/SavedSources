// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class ABaseCharacter;

UCLASS()
class ASCEND_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeConstruct() override;

	UPROPERTY()
	ABaseCharacter* PlayerCharacter = nullptr;


};
