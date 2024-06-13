// @2023 All rights reversed by Reverse-Alpha Studios


#include "HUD/EliminationAnnouncement.h"

#include "Components/TextBlock.h"

void UEliminationAnnouncement::SetEliminationAnnouncementText(FString AttackerName, FString VictimName)
{
	FString ElimAnnouncementText = FString::Printf(TEXT("%s Killed %s!"), *AttackerName, *VictimName);
	if (EliminationAnnouncementText)
	{
		EliminationAnnouncementText->SetText(FText::FromString(ElimAnnouncementText));
	}
}
