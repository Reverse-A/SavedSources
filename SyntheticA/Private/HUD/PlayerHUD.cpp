// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/PlayerHUD.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Characters/BaseCharacter.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "GameFramework/PlayerController.h"
#include "HUD/Announcement.h"
#include "HUD/DisplayManager.h"
#include "HUD/EliminationAnnouncement.h"


#pragma region Main/Functions
void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	OwningPlayer = GetOwningPlayerController();
}
#pragma endregion
#pragma region HUD
void APlayerHUD::AddEliminationAnnoucementToScreen(FString Attacker, FString Victim)
{
	OwningPlayer = OwningPlayer == nullptr ? GetOwningPlayerController() : OwningPlayer;

	if (OwningPlayer && EliminationAnnouncementClass)
	{
		UEliminationAnnouncement* EliminationAnnouncementWidget = CreateWidget<UEliminationAnnouncement>(OwningPlayer, EliminationAnnouncementClass);
		if (EliminationAnnouncementWidget)
		{
			EliminationAnnouncementWidget->SetEliminationAnnouncementText(Attacker, Victim);
			EliminationAnnouncementWidget->AddToViewport();

			for (UEliminationAnnouncement* Messages : ElimminationMessages)
			{
				if (Messages && Messages->EliminationAnnouncementBox)
				{
					UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Messages->EliminationAnnouncementBox);
					if (CanvasSlot)
					{
						FVector2D Position = CanvasSlot->GetPosition();

						FVector2D NewPosition(CanvasSlot->GetPosition().X, Position.Y - CanvasSlot->GetSize().Y);

						CanvasSlot->SetPosition(NewPosition);
					}
					else
					{
						LOG_WARNING("CANVASSLOT IS NOT LOADED UP!")
					}
				}
			}

			ElimminationMessages.Add(EliminationAnnouncementWidget);

			FTimerHandle EliminationMessageTimer;
			FTimerDelegate EliminationMessageDelegate;
			EliminationMessageDelegate.BindUFunction(this, FName("FinishTimer_EliminationAnnouncement"), EliminationAnnouncementWidget);

			GetWorldTimerManager().SetTimer
			(
				EliminationMessageTimer,
				EliminationMessageDelegate,
				EliminationAnnouncementTime,
				false
			);
		}
	}
}

void APlayerHUD::AddPlayerInterfaceToScreen()
{
	APlayerController* PlayerController = GetOwningPlayerController();

	if (PlayerController && PlayerScreenWidget)
	{
		DisplayManager = CreateWidget<UDisplayManager>(PlayerController, PlayerScreenWidget);
		DisplayManager->AddToViewport();
	}
}

void APlayerHUD::AddAerialPilotInterfaceToScreen()
{
	OwningPlayer = OwningPlayer == nullptr ? GetOwningPlayerController() : OwningPlayer;

	if (OwningPlayer && AerialPilotScreenWidget)
	{
		DisplayManager = CreateWidget<UDisplayManager>(OwningPlayer, AerialPilotScreenWidget);
		DisplayManager->AddToViewport();
	}
}

void APlayerHUD::AddAnnouncementToScreen()
{
	OwningPlayer = OwningPlayer == nullptr ? GetOwningPlayerController() : OwningPlayer;

	if (OwningPlayer && AnnouncementClass)
	{
		Announcement = CreateWidget<UAnnouncement>(OwningPlayer, AnnouncementClass);
		Announcement->AddToViewport();
	}
}

void APlayerHUD::FinishTimer_EliminationAnnouncement(UEliminationAnnouncement* MessageToRemove)
{
	if (MessageToRemove)
	{
		MessageToRemove->RemoveFromParent();
	}
}

void APlayerHUD::DrawHUD()
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwningPawn());
	if (BaseCharacter && BaseCharacter->GetPlayerIsEliminated())
	{
		return;
	}

	FVector2D ViewportSize;

	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		const FVector2D ViewportCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		FLinearColor CrosshairColor = FLinearColor::White;

		if (HUDPackage.CrosshairsCenter)
		{
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2D Spread(0.f, SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			FVector2D Spread(0.f, -SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
	}

	Super::DrawHUD();
}
void APlayerHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth	= Texture->GetSizeX();
	const float TextureHeight	= Texture->GetSizeY();

	const FVector2D TextureDrawPoint
	(
		ViewportCenter.X - (TextureWidth  / 2.0f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.0f + Spread.Y)
	);

	DrawTexture
	(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.0f,
		0.0f,
		1.0f,
		1.0f,
		CrosshairColor
	);
}
#pragma endregion