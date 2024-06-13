// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Weapon/WeaponTypes.h"
#include "CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNTHETICA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()
#pragma region Constructor-General
public:	
	UCombatComponent();

protected:
	friend class ABaseCharacter;


protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
#pragma endregion
#pragma region Classes
private:
	UPROPERTY()	class ABaseCharacter*				BaseCharacter;

	UPROPERTY()	class APlayerControllerManager*		PlayerControllerManager;

	UPROPERTY()	class APlayerHUD*					PlayerHUD;

#pragma endregion
#pragma region Equip Weapon
protected:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	class AWeapon* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeapon* SecondaryWeapon;

	void Equip_Weapon(AWeapon* WeaponToEquip);

	void Equip_PrimaryWeapon(AWeapon* WeaponToEquip);
	void Equip_SecondaryWeapon(AWeapon* WeaponToEquip);

	void Attach_WeaponToCharactersHand(AActor* Actor);
	void AttachWeaponToCharactersBackup(AActor* Actor);

	void CheckAndDrop_PrimariyWeapon();
	void DoSwap();

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_SecondaryWeapon();
public:
	bool IsWeaponEquipped();
	//TODO: void ThrowPrimaryWeapon(AWeapon Weapon);

	FORCEINLINE AWeapon* GetSecondaryWeapon() const { return SecondaryWeapon; }

#pragma endregion
#pragma region Equip Flag
private:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedFlag)
	class AFlag* EquippedFlag;

	void Equip_Flag(AFlag* FlagToEquip);

	UFUNCTION()
	void OnRep_EquippedFlag();

public:
	void Attach_FlagToCharacters(AActor* Flag);
	void Detach_FlagFromCharacters(AActor* Flag);
private:
	UPROPERTY(Replicated)
	bool bHoldingTheFlag = false;

public:
	FORCEINLINE bool Get_HoldingFlag() const { return bHoldingTheFlag; }
	FORCEINLINE AFlag* Get_EquippedFlag() const { return EquippedFlag; }

#pragma endregion
#pragma region Pick-Up
public:
	void PickupAmmunition(EWeaponType WeaponType, int32 AmmunitionAmount);
#pragma endregion
#pragma region Ammunition
private:
	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 MaxCarriedAmmunition = 60;

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();
	void InitializeCarriedAmmo();

	void UpdateCarriedAmmunition();
	void Update_AmmoValues();
	void Update_AmmoValues_Shotgun();

/*The initial ammunition quantity allocated for each category of weapons is contingent upon the specific type and model, with varying starting values designated based on the firearm's intended use and design specifications.*/
	UPROPERTY(EditAnywhere)
	int32 StartingAssaultRifleAmmunition	= 30;

	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmunition			= 32;

	UPROPERTY(EditAnywhere)
	int32 StartingSubMachhineGunAmmunition  = 40;

	UPROPERTY(EditAnywhere)
	int32 StartingScatterWeaponAmmunation	= 16;

	UPROPERTY(EditAnywhere)
	int32 StartingSniperRifleAmmunition		= 15;

	UPROPERTY(EditAnywhere)
	int32 StartingRocketLauncherAmmunition  = 2;

	UPROPERTY(EditAnywhere)
	int32 StartingGrenadeLauncherAmmunition = 6;
#pragma endregion
#pragma region Reloading
/* Reloadig*/
private:
	UPROPERTY(Replicated)
	bool bIsReloading;

public:
	bool CanReload();
	void Reload();

	UFUNCTION(BlueprintCallable)
	void FinishReload();

	UFUNCTION(Server, Reliable) 
	void Server_Reload();

/*Shotgun reloading*/
	UFUNCTION(BlueprintCallable)
	void ShotgunShellReload();

private:
	int32 AmountToReload();

public:
// Getter && Setter
	FORCEINLINE bool IsReloading() const { return bIsReloading; }
	FORCEINLINE void SetReload(bool InsertValue) { bIsReloading = InsertValue; }
#pragma endregion
#pragma region WeaponSwaping
private:
	UPROPERTY(Replicated)
	bool bWeaponSwaping = false;

	bool CanSwap = true;

	bool CanSwapWeapons();

	void WeaponSwapEnd();

	UFUNCTION(Server, Reliable)
	void Server_SetWeaponSwap(bool InsertValue);

	float SwapTimer;

	float CanSwapTimer;

public:
	FORCEINLINE void SetSwapWeapon(bool SetValue) { bWeaponSwaping = SetValue; }
	FORCEINLINE bool IsSwapingWeapon() const { return bWeaponSwaping; }
#pragma endregion
#pragma region Aiming-System
/* Aim System */
private:
	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bIsAiming = false;

	bool bAimButtonPressed = false;

	UFUNCTION()
	void OnRep_Aiming();

public:
	bool CanAim() const { return !IsReloading(); }

	void Set_Aiming(bool SetValue);

	UFUNCTION(Server, Reliable)
	void Server_SetAiming(bool SetValue);

	bool IsAiming() const { return bIsAiming; }
#pragma endregion
#pragma region Fire-System
/* Fire Weapon System */
public:
	void Set_FireButton(bool bActive);

	bool CanFire();
	void Fire();

	void Fire_ProjectileWeapon();
	void Fire_HitScanWeapon();
	void Fire_ScatterTypeWeapon();


	void PlayFireMontageAndCameraShake();

	void Local_Fire(const FVector_NetQuantize& TraceHitTarget);
	void Local_ScatterTypeFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire(const FVector_NetQuantize& TraceHitTarget, float FireDelay);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ScatterTypeFire(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ScatterTypeFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

private:
	bool bFireButtonActive;

private:
	UPROPERTY(EditAnywhere, Category = WeaponProperties)
	class UAnimMontage* FireWeaponMontage;
private:
	FTimerHandle FireTimer;

	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

public:
	FORCEINLINE bool IsFireButtonActive() const { return bFireButtonActive; }

#pragma endregion
#pragma region TraceHit And Crosshair
/* Trace Hit */
public:
	FVector HitTarget;
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
private:
	bool bIsActorTargetedByCrosshair;

public:
	FORCEINLINE bool IsActorTargetedbyCrosshair() const { return bIsActorTargetedByCrosshair; }
#pragma endregion
#pragma region Hit
/*Hit Reactions*/
private:
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* HitReactionMontage;

public:
	void PlayMontage_HitReaction();
#pragma endregion
#pragma region ShieldBreak
public:
	void ShieldBreak();

	UFUNCTION(Server, Reliable)
	void Server_PlayShieldBreak();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayShieldBreak();

private:
	void PlayShieldBreakEffect();
	void PlaySound_ShieldBreak();

private:
	UPROPERTY(EditAnywhere, Category = "CombatProperties")
	class UParticleSystem* ShieldBreakParticle;

	UPROPERTY(EditAnywhere, Category = "CombatProperties")
	class USoundCue* ShieldBreakSound;
#pragma endregion

#pragma region PlaySounds
public:
	void PlaySound_EquipWeapon(AWeapon* Weapon);
#pragma endregion
};
