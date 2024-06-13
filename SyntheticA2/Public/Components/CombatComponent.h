// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUD/PlayerHUD.h"
#include "Weapon/WeaponTypes.h"
#include "GeneralTypes/CombatTypes.h"
#include "CombatComponent.generated.h"

#pragma region | Other Classes |
class ABaseCharacter;
class APlayerControllerManager;
class APlayerHUD;
class AWeapon;

class USkeletalMeshComponent;
class UEnhancedMovementComponent;
#pragma endregion

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNTHETICA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region | Constructors | 
public:
	UCombatComponent();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	friend class ABaseCharacter;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

#pragma region Classes
private:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

	UPROPERTY()
	APlayerControllerManager* PlayerControllerManager;

	USkeletalMeshComponent* CharacterSkeletalMesh;

	UEnhancedMovementComponent* EnhancedMovementComponent;

#pragma endregion

#pragma region | Weapon |
	/*---------------------------------------------------------------------------------------------------------------------------------------*/
	//-------------------------//--------------------------------------[Synethica]------------------------------//---------------------------*/
	/*----------------------------------------------------------------[  Weapons  ]---------------------------------------------------------*/
	/*---------------------------------------------------------------------------------------------------------------------------------------*/
public:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	void EquipWeapon(AWeapon* WeaponToEquip);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	bool IsWeaponEquipped();

	UFUNCTION(Server, Reliable)
	void Server_EquipButtonPressed();

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	void SetOverlappingWeapon(AWeapon* Weapon);

	AWeapon* GetEquippedWeapon();

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
#pragma endregion

#pragma region Weapon System
	/* Fire System */
public:
	bool CanFire();
	void SetFireButtonTrigger(bool bTriggered);

	FORCEINLINE bool IsFireButtonPressed() const { return bFireButtonPressed; }

protected:
	void Fire();

	UFUNCTION(Server, Reliable)
	void Server_Fire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire(const FVector_NetQuantize& TraceHitTarget);
private:
	bool bFireButtonPressed;
	bool bCanFire = true;

/* Fire Animation */
private:
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;

	void PlayFireMontage(bool bIsAiming);

private:
	FTimerHandle FireTimer;
	void StartFireTimer();
	void FireTimerFinished();

	/* Reload */
//private:
//	UPROPERTY(EditAnywhere, Category = Combat)
//	class UAnimMontage* ReloadMontage;
//
	UPROPERTY(Replicated)
	bool bIsReloading;
//
//public:
	void Reload();
	UFUNCTION(Server, Reliable)
	void Server_Reload();
//
//	void HandleReload();
//
	uint32 AmountToReload();
//
//	void PlayReloadMontage();
//	UFUNCTION(BlueprintCallable)
//	void FinishReloading();
//
//	FORCEINLINE bool GetIsReloading() const { return bIsReloading; }

/* Ammunation */
public:
	// Carried ammo for the currently-equipped weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();
	void InitializeCarriedAmmo();
	void UpdateAmmoValues();
	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo = 12;

	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo = 7;

	UPROPERTY(EditAnywhere)
	int32 StartingSMGAmmo = 25;

	UPROPERTY(EditAnywhere)
	int32 StartingAssaultRifleAmmo = 30;

	UPROPERTY(EditAnywhere)
	int32 StartingSniperRileAmmo = 5;

	UPROPERTY(EditAnywhere)
	int32 StartingGrenadeLauncherAmmo = 3;

	UPROPERTY(EditAnywhere)
	int32 StartingRocketLauncherAmmo = 2;

private:
	//UPROPERTY(ReplicatedUsing = OnRep_CombatTypes)
	//ECombatTypes CombatTypes = ECombatTypes::ECT_Unoccupied;

	//UFUNCTION()
	//void OnRep_CombatTypes();
#pragma endregion

#pragma region Hit System
	/* Hit Reactions */
private:
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* HitReactionMontage;

public:
	void PlayHitReactionMontage();
#pragma endregion

#pragma endregion

#pragma region Aim
private:
	UPROPERTY(Replicated)
	bool bAiming;

protected:
	void SetAiming(bool bIsAiming);
	UFUNCTION(Server, Reliable)
	void Server_SetAiming(bool bIsAiming);

public:
	void ShowSniperScopeWidget(bool bShowScope);
#pragma endregion

#pragma region | HUD | 
public:
	FVector HitTarget;
	UPROPERTY()
	class APlayerHUD* PlayerHUD;

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
#pragma endregion

#pragma region CombatYpes
private:
	UPROPERTY(ReplicatedUsing = OnRep_CombatTypes)
	ECombatTypes CombatTypes = ECombatTypes::ECT_Unoccupied;

	UFUNCTION()
	void OnRep_CombatTypes();
#pragma endregion
};
