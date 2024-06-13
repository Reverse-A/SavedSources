// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ascend/Weapon/WeaponStructureBase.h"

#include "PlayerControllerModule.generated.h"

class APlayerHUD;

UCLASS()
class ASCEND_API APlayerControllerModule : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerControllerModule(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	APlayerHUD* GetPlayerHUD() const;
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	APlayerHUD* PlayerHUD;


#pragma region HUD/Functions
public:

	void SetHUDHealth(float Health, float MaxHealth, bool bAlive);
	void SetHUDShield(float Shield, float MaxShield, bool bAlive);
	void SetHUDStamina(float Stamina, float MaxStamina, bool bAlive);

	void SetHUDCredits(float Credits);

	void SetHUDWeaponAmmunition(int32 Ammunition);
	void SetHUDCarriedAmmunition(int32 CarriedAmmunition);
	void SetHUDRangedWeaponType(ERangedWeaponType RangedWeaponType);
	void SetHUDMeleeWeaponType(EMeleeWeaponType MeleeWeaponType);

	void SetHUDExperience(float Experience, float LevelExperienceCap, int32 CurrentPlayerLevel, bool bAlive);

	void SetHUDQuests();
};
