// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|


#include "HUD/PlayerHUD.h"

#include "HUD/PlayerHUDPanel.h"
#include "GameFramework/PlayerController.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerHUD::AddPlayerHUDPanel()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController)
	{
		if (PlayerHUDPanelClass)
		{
			PlayerHUDPanel = CreateWidget<UPlayerHUDPanel>(PlayerController, PlayerHUDPanelClass);
			PlayerHUDPanel->AddToViewport();
		}
	}
}

//void APlayerHUD::AddAnnouncement()
//{
//	APlayerController* PlayerController = GetOwningPlayerController();
//	if (PlayerController && AnnouncementClass)
//	{
//		if (PlayerHUDPanelClass)
//		{
//			Announcement = CreateWidget<UAnnouncement>(PlayerController, AnnouncementClass);
//			Announcement->AddToViewport();
//		}
//	}
//}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;

	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		const FVector2D ViewportCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		FLinearColor CrosshairColor = HUDPackage.CrosshairsColor;

		if (HUDPackage.CrosshairsCenter)
		{
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, Spread, CrosshairColor);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, CrosshairColor);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, Spread, CrosshairColor);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2D Spread(0.f, -SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, Spread, CrosshairColor);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			FVector2D Spread(0.f, SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter, Spread, CrosshairColor);
		}
	}
}

void APlayerHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const FVector2D TextureDrawPoint
	(
		ViewportCenter.X - (TextureWidth / 2.0f) + Spread.X,
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