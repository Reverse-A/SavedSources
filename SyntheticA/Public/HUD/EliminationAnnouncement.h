// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EliminationAnnouncement.generated.h"

class UTextBlock;
class UHorizontalBox;
class UCanvasPanelSlot;
UCLASS()
class SYNTHETICA_API UEliminationAnnouncement : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetEliminationAnnouncementText(FString AttackerName, FString VictimName);

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* EliminationAnnouncementBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock*	EliminationAnnouncementText;
};
