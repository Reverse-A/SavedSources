// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInventory.generated.h"

class USizeBox;
class UArsenalComponent;

class ABaseCharacter;

UCLASS()
class ASCEND_API UPlayerInventory : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USizeBox* ItemContainer;

private:
	UPROPERTY()
	ABaseCharacter* PlayerCharacter;

	UPROPERTY()
	UArsenalComponent* ArsenalComponent;
};
