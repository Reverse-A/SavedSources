// // @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerHUD.h"

#include "Ascend/Character/BaseCharacter.h"
#include "Ascend/Character/ShortDebugMacros.h"
#include "Ascend/Component/ArsenalComponent.h"

#include "Ascend/UI/Player/PlayerMainScreen.h"
#include "Blueprint/UserWidget.h"

#include "GameFramework/PlayerController.h"

void APlayerHUD::BeginPlay()
{
	OwnerPlayerController = GetOwningPlayerController();
	AddPlayerMainScreenToScreen();

	if (OwnerPlayerController)
	{
		BaseCharacter = Cast<ABaseCharacter>(OwnerPlayerController->GetCharacter());
	}
}


void APlayerHUD::AddPlayerMainScreenToScreen()
{
	OwnerPlayerController = OwnerPlayerController == nullptr ? GetOwningPlayerController() : OwnerPlayerController;

	if (OwnerPlayerController && PlayerMainScreenWidget)
	{
		PlayerMainScreen = CreateWidget<UPlayerMainScreen>(OwnerPlayerController, PlayerMainScreenWidget);
		PlayerMainScreen->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("PlayerMainScreen Initiated"));
	}
	else
	{
#if CHECK_OWNER_PLAYER_CONTROLLER == 1
		if (OwnerPlayerController == nullptr)
		{
			LOG_TO_DESKTOP("OwnerPlayerController nullptr here: ");
		}
#endif 

#if CHECK_PLAYER_MAIN_SCREEN == 1
		if (PlayerMainScreen == nullptr)
		{
			LOG_TO_DESKTOP("PlayerMainScreen nullptr here: ");
		}
#endif
	}
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;

	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		const FVector2D ViewportCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
		
		float SpreadScaled = CrosshairSpreadMax * CrosshairPackage.CrosshairSpreadMultiplier;

		FLinearColor CrosshairColor = FLinearColor::White;

		bool Phase_1 = 
			BaseCharacter->GetArsenalSystem()->GetPrimaryEquipWeapon() == nullptr;

		bool Phase_2 =
			BaseCharacter->GetArsenalSystem()->GetPrimaryEquipWeapon() != nullptr &&
			BaseCharacter->GetVelocity().Size() <= 400.0f &&
			BaseCharacter->GetAimButtonPressed() == false;

		bool Phase_3 =
			BaseCharacter->GetArsenalSystem()->GetPrimaryEquipWeapon() != nullptr &&
			BaseCharacter->GetVelocity().Size() > 400.0f;

		bool Phase_4 =
			BaseCharacter->GetArsenalSystem()->GetPrimaryEquipWeapon() != nullptr && 
			BaseCharacter->GetAimButtonPressed();


		if (Phase_1)
		{
			if (CrosshairPackage.CrosshairCenter)
			{
				FVector2D Spread(0.0f, 0.0f);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairCenter, ViewportCenter, Spread, CrosshairPackage.CrosshairColor);
			}
		}
		else if(Phase_2)
		{
			if (CrosshairPackage.CrosshairCenter)
			{
				FVector2D Spread(0.0f, 0.0f);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairCenter, ViewportCenter, Spread, CrosshairPackage.CrosshairColor);
			}
			if (CrosshairPackage.CrosshairRight)
			{
				FVector2D Spread(SpreadScaled, 0.f);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairRight, ViewportCenter, Spread, CrosshairPackage.CrosshairColor);
			}
			if (CrosshairPackage.CrosshairLeft)
			{
				FVector2D Spread(-SpreadScaled, 0.f);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairLeft, ViewportCenter, Spread, CrosshairPackage.CrosshairColor);
			}
			if (CrosshairPackage.CrosshairTop)
			{
				FVector2D Spread(0.f, -SpreadScaled);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairTop, ViewportCenter, Spread, CrosshairPackage.CrosshairColor);
			}
			if (CrosshairPackage.CrosshairBottom)
			{
				FVector2D Spread(0.f, SpreadScaled);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairBottom, ViewportCenter, Spread, CrosshairPackage.CrosshairColor);
			}
		}
		else if (Phase_3)
		{
			if (CrosshairPackage.CrosshairCenter)
			{
				FVector2D Spread(0.0f, 0.0f);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairCenter, ViewportCenter, Spread, CrosshairPackage.CrosshairColor);
			}
		}
		else if (Phase_4)
		{
			FLinearColor TransparentColour(0, 0, 0, 0);

			if (CrosshairPackage.CrosshairCenter)
			{
				FVector2D Spread(0.0f, 0.0f);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairCenter, ViewportCenter, Spread, TransparentColour);
			}
			if (CrosshairPackage.CrosshairRight)
			{
				FVector2D Spread(SpreadScaled, 0.f);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairRight, ViewportCenter, Spread, TransparentColour);
			}
			if (CrosshairPackage.CrosshairLeft)
			{
				FVector2D Spread(-SpreadScaled, 0.f);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairLeft, ViewportCenter, Spread, TransparentColour);
			}
			if (CrosshairPackage.CrosshairTop)
			{
				FVector2D Spread(0.f, -SpreadScaled);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairTop, ViewportCenter, Spread, TransparentColour);
			}
			if (CrosshairPackage.CrosshairBottom)
			{
				FVector2D Spread(0.f, SpreadScaled);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairBottom, ViewportCenter, Spread, TransparentColour);
			}
		}
		else
		{
			FLinearColor TransparentColour(0, 0, 0, 0);

			if (CrosshairPackage.CrosshairCenter)
			{
				FVector2D Spread(0.0f, 0.0f);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairCenter, ViewportCenter, Spread, TransparentColour);
			}
			if (CrosshairPackage.CrosshairRight)
			{
				FVector2D Spread(SpreadScaled, 0.f);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairRight, ViewportCenter, Spread, TransparentColour);
			}
			if (CrosshairPackage.CrosshairLeft)
			{
				FVector2D Spread(-SpreadScaled, 0.f);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairLeft, ViewportCenter, Spread, TransparentColour);
			}
			if (CrosshairPackage.CrosshairTop)
			{
				FVector2D Spread(0.f, -SpreadScaled);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairTop, ViewportCenter, Spread, TransparentColour);
			}
			if (CrosshairPackage.CrosshairBottom)
			{
				FVector2D Spread(0.f, SpreadScaled);
				DrawCrosshairToScreen(CrosshairPackage.CrosshairBottom, ViewportCenter, Spread, TransparentColour);
			}
		}
	}
}

void APlayerHUD::DrawCrosshairToScreen(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth  = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const float Divisor = 2.0f;

	const FVector2D TextureDrawPoint
	(
		ViewportCenter.X - (TextureWidth / Divisor) + Spread.X,
		ViewportCenter.Y - (TextureHeight / Divisor) + Spread.Y
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
