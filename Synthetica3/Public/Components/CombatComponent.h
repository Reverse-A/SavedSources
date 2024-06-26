// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponTypes.h"
#include "Synthetica/EnumTypes/CombatTypes.h"

#include "Components/ActorComponent.h"
#include "HUD/PlayerHUD.h"
#include "CombatComponent.generated.h"

#pragma region | Other Classes |
class ABaseCharacter;
class APlayerControllerManager;
class APlayerHUD;
class AWeapon;

class USkeletalMeshComponent;
class UEnhancedMovementComponent;
#pragma endregion
#pragma region | Class |
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNTHETICA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()
#pragma endregion
#pragma region | Constructors | 
public:	
	UCombatComponent();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	friend class ABaseCharacter;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;
	UPROPERTY()
	APlayerControllerManager* PlayerControllerManager;

	USkeletalMeshComponent* CharacterMesh;

	UEnhancedMovementComponent* EnhancedMovement;
#pragma endregion

#pragma region | HUD | 
public:
	FVector HitTarget;
	UPROPERTY()
	class APlayerHUD* PlayerHUD;

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
#pragma endregion

#pragma region | Equip System |
public:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	void EquipWeapon(AWeapon* WeaponToEquip);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	bool IsWeaponEquipped();

	UFUNCTION(Server, Reliable)
	void Server_EquipButtonPressed();

#pragma endregion

#pragma region | Aim |
/* Aiming */
private:
	UPROPERTY(Replicated)
	bool bAiming;

protected:
	void SetAiming(bool bIsAiming);
	UFUNCTION(Server, Reliable)
	void Server_SetAiming(bool bIsAiming);

#pragma endregion

#pragma region | Weapon System |
/* Fire System */
private:
	void Fire();
	bool bFireButtonPressed;

	bool bCanFire = true;
public:
	void SetFireButtonTrigger(bool bTriggered);
	FORCEINLINE bool IsFireButtonPressed() const { return bFireButtonPressed; }
protected:
	UFUNCTION(Server, Reliable)
	void Server_Fire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire(const FVector_NetQuantize& TraceHitTarget);
private:
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;

	void PlayFireMontage(bool bIsAiming);

private:
	FTimerHandle FireTimer;

	void StartFireTimer();
	void FireTimerFinished();

public:
	bool CanFire();

/* Reload */
public:
	void Reload(); 
	UFUNCTION(Server, Reliable)
	void Server_Reload();

	void HandleReload();

	uint32 AmountToReload();


	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* ReloadMontage;

	void PlayReloadMontage();
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	FORCEINLINE bool GetIsReloading() const { return bIsReloading; }

private:
	UPROPERTY(Replicated)
	bool bIsReloading;

// Ammunation System
public:
	// Carried ammo for the currently-equipped weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo		 = 12;

	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo		 = 7;

	UPROPERTY(EditAnywhere)
	int32 StartingSMGAmmo			 = 25;

	UPROPERTY(EditAnywhere)
	int32 StartingAssaultRifleAmmo	 = 30;

	UPROPERTY(EditAnywhere)
	int32 StartingSniperRileAmmo	 = 5;

	UPROPERTY(EditAnywhere)
	int32 StartingGrenadeLauncherAmmo= 3;

	UPROPERTY(EditAnywhere)
	int32 StartingRocketLauncherAmmo = 2;

	void InitializeCarriedAmmo();

	void UpdateAmmoValues();

private:
	UPROPERTY(ReplicatedUsing = OnRep_CombatTypes)
	ECombatTypes CombatTypes = ECombatTypes::ECT_Unoccupied;

	UFUNCTION()
	void OnRep_CombatTypes();
#pragma endregion

#pragma region Hit System
/* Hit Reactions */
private:
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* HitReactionMontage;

public:
	void PlayHitReactionMontage();
#pragma endregion

#pragma region | Server |

#pragma endregion

#pragma region | Getter & Setter |
public:
	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
#pragma endregion
};
