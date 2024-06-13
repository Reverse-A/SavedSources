// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUD/PlayerHUD.h"
#include "CameraManager.generated.h"

class ABaseCharacter;
class APlayerControllerManager;
class APlayerHUD;

class UCombatComponent;
class UEnhancedMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNTHETICA_API UCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCameraManager();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY() ABaseCharacter*				BaseCharacter;
	UPROPERTY() APlayerControllerManager*	PlayerControllerManager;
	UPROPERTY() APlayerHUD*					PlayerHUD;

protected:
	UPROPERTY() UCombatComponent*			CombatComponent;
	UPROPERTY() UEnhancedMovementComponent*	EnhancedMovementComponent;

	/* Camera Functionalies */
private:
	bool bActorVisible;

	void HideActor();
	void SetActorMeshesVisibility(bool bVisible);

#pragma region Crosshairs
private:
	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CircleTypeCrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CircleTypeCrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CircleTypeCrosshairsRight;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* RPGTypeCrosshairsCenter;

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

public:
	// This functions is not using right now// 
	void InitialCrosshair();

	void SetHUDCrosshairs(float DeltaTime);
	FORCEINLINE void SetCrosshairShootingFactor(float InsertValue) { CrosshairShootingFactor = InsertValue; }

#pragma endregion

/* Aim FOV */
private:
	float CurrentFOV;

	float DefaultFOV;
	void InterpFOV(float DeltaTime);

	void SprintFOV(float DeltaTime);

/* Crouch CameraPos Interpolation */
private:
	void Crouch_InterpolateCameraPosition(float DeltaTime);

/*Camera Shakes*/
public:
	void PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass);
};
