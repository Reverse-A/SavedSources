// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	UTexture2D*  CrosshairsCenter;
	UTexture2D*  CrosshairsLeft;
	UTexture2D*  CrosshairsRight;
	UTexture2D*  CrosshairsTop;
	UTexture2D*  CrosshairsBottom;
	FLinearColor CrosshairsColor;
	float		 CrosshairSpread;
};

UCLASS()
class SYNTHETICA_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
#pragma region Main/Functions
protected:
	virtual void BeginPlay() override;
#pragma endregion

#pragma region Classes
private:
	UPROPERTY()
	class APlayerController* OwningPlayer;

	UPROPERTY(EditAnywhere, Category = "Classes")
	class TSubclassOf<class UUserWidget> PlayerScreenWidget;

	UPROPERTY(EditAnywhere, Category = "Classes")
	class TSubclassOf<class UUserWidget> AerialPilotScreenWidget;

	UPROPERTY(EditAnywhere, Category = "Classes")
	class TSubclassOf<class UUserWidget> ArmoredScreenWidget;

	UPROPERTY(EditAnywhere, Category = "Classes")
	class TSubclassOf<class UUserWidget> TransporterScreenWidget;

	UPROPERTY(EditAnywhere, Category = "Classes")
	TSubclassOf<UUserWidget> AnnouncementClass;

	UPROPERTY(EditAnywhere, Category = "Classes")
	TSubclassOf<class UEliminationAnnouncement> EliminationAnnouncementClass;

	FHUDPackage HUDPackage;
public:
	UPROPERTY()
	class UDisplayManager* DisplayManager;

	UPROPERTY()
	class UAnnouncement* Announcement;

#pragma endregion

#pragma region HUD
private:
	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.0f;

	UPROPERTY(EditAnywhere)
	float EliminationAnnouncementTime = 5.f;
public:
	virtual void DrawHUD() override;
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	void AddPlayerInterfaceToScreen();
	void AddAerialPilotInterfaceToScreen();

	//void AddArmoredInterfaceToScreen();
	//void AddTransporterInterfaceToScreen();
	
	void AddAnnouncementToScreen();

	/*Elimination*/
	UPROPERTY()
	TArray<UEliminationAnnouncement*> ElimminationMessages;

	void AddEliminationAnnoucementToScreen(FString Attacker, FString Victim);

	UFUNCTION()
	void FinishTimer_EliminationAnnouncement(UEliminationAnnouncement* MessageToRemove);
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	FORCEINLINE void SetCrosshairColor(FLinearColor NewColor) { HUDPackage.CrosshairsColor = NewColor; }
#pragma endregion
};
