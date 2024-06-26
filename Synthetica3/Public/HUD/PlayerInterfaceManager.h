// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInterfaceManager.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class SYNTHETICA_API UPlayerInterfaceManager : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EquipWeaponText;
	
};
