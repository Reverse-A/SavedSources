// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Synthetica/Weapons/RangedWeaponTypes.h"
#include "Synthetica/HUD/BaseHUD.h"
#include "CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNTHETICA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class ABaseCharacter;

	void EquipWeapon(class AWeapon* WeaponToEquip);
	void UnEquipWeapon(class AWeapon* WeaponToUnEquip);
	void DropWeapon();
	void SetAiming(bool bIsAiming);
	void UpdateCarriedAmmunition();
	void PickupAmmunition(ERangedWeaponTypes RangeWeaponType, int32 AmmunationAmount);

	void Fire();

	bool bAiming;

protected:
	virtual void BeginPlay() override;

	class ABaseHUD* HUD;
	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseThrowableGrenades> BaseGrenadeClass;

	void FireButtonPressed(bool bPressed);
	void MeleeAttackPressed(bool bPressed);
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);
	void InterpFOV(float DeltaTime);

	// Grenade
	void ThrowGrenade();

	UFUNCTION(BlueprintCallable)
	void LaunchGrenade();

	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeFinished();
	void ThrowGrenadeFinishedTimer();

	void UpdateHUDGrenades();

	//Grenade Variables
	FTimerHandle ThrowGrenadeTimerHandle;
	bool bGrenadeIsLaunched = false;

	void ShowAttachedGrenade(bool bShowGrenade);
	void CombatStateLogic();

	void IfWeaponEquipped_DropEquippedWeapon();
	void AttachActorToRightHand(AActor* ActorToAttach);
	void AttachActorToLeftHand(AActor* ActorToAttach);
	void UpdateCarriedAmmo();
	void PlayEquippedWeaponSound();
	void ReloadEmptyMag();

private:
	class ABaseCharacter* BaseCharacter;
	class ABasePlayerController* BaseController;


	 
	class AWeapon* EquippedWeapon;
	AWeapon* DisarmedWeapon;

	ECombatState CombatState;

	FVector HitTarget;

	bool bIsSprinting;
	bool bFireButtonPressed;
	bool bAttackButtonPressed;

	// HUD and Crosshairs

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	float DefaultFOV;
	float CurrentFOV;
	float ZoomedFOV = 30.f;

	FTimerHandle FireTimer;
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();

	int32 CarriedAmmo; 
	int32 AmountToReload();

	TMap<ERangedWeaponTypes, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 MaxCarriedAmmo = 160;

	UPROPERTY(EditAnywhere)
	int32 AssaultRiffleInitialAmmunation = 45;

	UPROPERTY(EditAnywhere)
	int32 RocketLauncherInitialAmmunation = 4;

	UPROPERTY(EditAnywhere)
	int32 PistolInitialAmmunation = 15;

	UPROPERTY(EditAnywhere)
	int32 SMGInitialAmmunation = 30;

	UPROPERTY(EditAnywhere)
	int32 ShotgunInitialAmmunation = 7;

	UPROPERTY(EditAnywhere)
	int32 SniperInitialAmmunation = 8;

	UPROPERTY(EditAnywhere)
	int32 GrenadeLauncherInitialAmmunation = 5;

	UPROPERTY(EditAnywhere)
	int32 Grenades = 4;

	UPROPERTY(EditAnywhere)
	int32 MaxGrenades = 4;

	void InitializeCarriedAmmo();


public:	
	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
	FORCEINLINE int32 GetGrenades() const { return Grenades; }

};
