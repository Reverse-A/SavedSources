// // @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

USTRUCT(BlueprintType)
struct FCrosshairPackage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor CrosshairColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrosshairSpreadMultiplier;

	FCrosshairPackage()
		: CrosshairCenter(nullptr)
		, CrosshairRight(nullptr)
		, CrosshairLeft(nullptr)
		, CrosshairTop(nullptr)
		, CrosshairBottom(nullptr)
		, CrosshairColor(FLinearColor::White)
		, CrosshairSpreadMultiplier(0.0f)
	{

	}

public:
	UTexture2D* GetCrosshairCenter()	 const { return CrosshairCenter; }
	UTexture2D* GetCrosshairRight()		 const { return CrosshairRight; }
	UTexture2D* GetCrosshairLeft()		 const { return CrosshairLeft; }
	UTexture2D* GetCrosshairTop()		 const { return CrosshairTop; }
	UTexture2D* GetCrosshairBottom()	 const { return CrosshairBottom; }
	float GetCrosshairSpreadMultiplier() const { return CrosshairSpreadMultiplier; }

	void SetCrosshairCenter	(UTexture2D* Texture) { CrosshairCenter = Texture; }
	void SetCrosshairRight	(UTexture2D* Texture) { CrosshairCenter = Texture; }
	void SetCrosshairLeft	(UTexture2D* Texture) { CrosshairCenter = Texture; }
	void SetCrosshairTop	(UTexture2D* Texture) { CrosshairCenter = Texture; }
	void SetCrosshairBottom	(UTexture2D* Texture) { CrosshairCenter = Texture; }

	void SetCrosshairColor(FLinearColor NewColor) { CrosshairColor = NewColor; }
};


#define CHECK_OWNER_PLAYER_CONTROLLER 0
#define CHECK_PLAYER_MAIN_SCREEN 0

class ABaseCharacter;
class APlayerController;
class UPlayerMainScreen;
class UUserWidget;

UCLASS()
class ASCEND_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

	UPROPERTY()
	APlayerController* OwnerPlayerController;

	UPROPERTY()
	UPlayerMainScreen* PlayerMainScreen;

	UPROPERTY(EditAnywhere, Category = "Classes")
	class TSubclassOf<UUserWidget> PlayerMainScreenWidget;

private:
	/*HUD*/
	UPROPERTY(EditAnywhere, Category = DeveloperProperties)
	FCrosshairPackage CrosshairPackage;

	/*Crosshair*/
	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.0f;

	void DrawCrosshairToScreen(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	/*Widgets*/
	void AddPlayerMainScreenToScreen();

public:
	virtual void DrawHUD() override;

public:
	FORCEINLINE FCrosshairPackage GetCrosshairPackage() const { return CrosshairPackage; }
};
