// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "Weapon.generated.h"

class ABaseCharacter;
class APlayerControllerManager;

class USkeletalMeshComponent;
class USphereComponent;
class USoundCue;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial				UMETA(DisplayName = "Initial State"),
	EWS_PrimaryEquipped		UMETA(DisplayName = "PrimaryEquipped"),
	EWS_SecondaryEquipped	UMETA(DisplayName = "SecondaryEquipped"),
	EWS_Dropped				UMETA(DisplayName = "Dropped"),

	EWS_MAX					UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EFireType : uint8
{
	EFT_HitScan					UMETA(DisplayName = "HitScanWeapon"),
	EFT_Projectile				UMETA(DisplayName = "ProjectileWeapon"),
	EFT_ScatterType				UMETA(DisplayName = "ScatterWeapon"),

	EWS_MAX						UMETA(DisplayName = "DefaultMAX")
};
#pragma region AWeapon
UCLASS()
class SYNTHETICA_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

#pragma region Classes
protected:
	UPROPERTY() ABaseCharacter*				BaseOwnerCharacter;
	UPROPERTY() APlayerControllerManager*	PlayerOwnerController;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	EWeaponType WeaponType;
#pragma endregion
#pragma region MainFunctionalities
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


/*NetWorking*/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

/*Owner*/
	virtual void OnRep_Owner() override;

public:
	void Drop_Weapon();
	void SetHUDAmmo();

#pragma endregion
#pragma region Overlap
protected:
	UFUNCTION()
	virtual void OnSphereOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);
#pragma endregion
#pragma region Blueprint Components || Other Components
private:
	UPROPERTY(VisibleAnywhere, Category = "Developer Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Developer Properties")
	USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, Category	= "Developer Properties")
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, Category	= "Developer Properties")
	USoundCue* EmptyTriggerSound;
#pragma endregion
#pragma region WeaponState
private:
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Developer Properties")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	EFireType FireType;

protected:
	virtual void OnWeaponEquippedPrimary();
	virtual void OnWeaponEquippedSecondary();
	virtual void OnWeaponDropped();
	virtual void OnWeaponStateSet();

	UFUNCTION()
	void OnRep_WeaponState();

public:
	void SetWeaponState(EWeaponState State);

#pragma endregion
#pragma region Eject-System
private:
	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	TSubclassOf<class AAmmoEject> AmmoEjectClass;
#pragma endregion
#pragma region Ammunition
private:
	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	int32 Ammo;

	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	int32 MagazineCapacity;

	// The number of unprocessed server requestes for Ammo. Incremented in Spendround, decrementend in client update ammo
	int32 Sequence = 0;

	void SpendRound();

	UFUNCTION(Client, Reliable)
	void Client_UpdateAmmo(int32 ServerAmmo);

	UFUNCTION(Client, Reliable)
	void Client_AddAmmo(int32 AmmoToAdd);
public:
	void AddAmmo(int32 AmmoToAdd);
#pragma endregion
#pragma region Fire-Systems
/* Fire- Systems */
public:
	virtual void Fire(const FVector& HitTarget);

	FVector TraceEndWithScatter(const FVector& HitTarget);
protected:
	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	bool bAutomatic = true;

	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	bool bUseScatter = false;

	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	float HeadShotDamage = 40.0f;

	/*Zoom Properties*/
	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	float ZoomedFOV = 45.0f;

	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	float ZoomInterpSpeed = 15.0f;

	/* Trace end with scatter */
	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Developer Properties")
	float SphereRadius = 75.f;
#pragma endregion
#pragma region Equipping||Destroying
private:
	bool bWeaponEquipped;
	bool bDestroyWeapon = false;

public:
	bool GetWeaponEquipped() const { return bWeaponEquipped; }
	void SetWeaponEquipped(bool Value) { bWeaponEquipped = Value; }

	bool Get_DestroyWeapon() const		{ return bDestroyWeapon; }
	void SetDestroyWeapon(bool Value)	{ bDestroyWeapon = Value; }
#pragma endregion
#pragma region ServerSideRewind
protected:
	UPROPERTY(Replicated, EditAnywhere)
	bool bUseServerSideRewind = false;

public:
	FORCEINLINE bool ServerSideRewind() const { return bUseServerSideRewind; }
	FORCEINLINE void SetServerSideRewind(bool SetValue) { bUseServerSideRewind = SetValue; }
#pragma endregion


#pragma region Ping 
private:
	UFUNCTION()
	void OnPingTooHigh(bool bPingTooHigh);

#pragma endregion

#pragma region Get&Set
	/*Getters*/
public:
	FORCEINLINE bool IsEmpty() { return Ammo <= 0; };
	FORCEINLINE bool IsFull() { return Ammo == MagazineCapacity; };

	FORCEINLINE EWeaponType				Get_WeaponType()			const { return WeaponType; }
	FORCEINLINE EFireType				GetFireType()				const { return FireType; }

	FORCEINLINE USphereComponent*		GetAreaSphere()				const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh()	 			const { return WeaponMesh; }
	FORCEINLINE USoundCue*				GetEquipSound()				const { return EquipSound; }
	FORCEINLINE USoundCue*				GetEmptyTriggerSound()		const { return EmptyTriggerSound; }

	FORCEINLINE int32					Get_Ammo()					const { return Ammo; }
	FORCEINLINE int32					Get_MagCapacity()			const { return MagazineCapacity; }
	FORCEINLINE float					GetZoomedFOV()				const { return ZoomedFOV; }
	FORCEINLINE float					GetZoomInterpSpeed()		const { return ZoomInterpSpeed; }

	FORCEINLINE float					GetDamage()					const { return Damage; }
	FORCEINLINE float					GetHeadShotDamage()			const { return HeadShotDamage; }

	FORCEINLINE bool					GetIsScatterWeaponType()	const { return bUseScatter; }
	FORCEINLINE bool					GetIsAutomaticWeapon()		const { return bAutomatic; }

	FORCEINLINE float					GetFireDelay()				const { return FireDelay; }
#pragma endregion
};
#pragma endregion