// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "Weapon.generated.h"

class ABaseCharacter;
class USkeletalMeshComponent;
class USphereComponent;
class UTexture2D;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial		UMETA(DisplayName = "Initial State"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),
	EWS_Dropped		UMETA(DisplayName = "Dropped"),

	EWS_MAX			UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class SYNTHETICA_API AWeapon : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class ABaseCharacter* BaseOwnerCharacter;

	UPROPERTY()
	class APlayerControllerManager* PlayerOwnerController;

public:
	AWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnRep_Owner() override;

	void SetHUDAmmo();

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


public:
	void SetDisplayEquipWeapon(bool bShowWidget);
	virtual void Fire(const FVector& HitTarget);

	void DropWeapon();
	void ThrowWeapon();

public:
	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	bool bAutomaticWeapon = true;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float FireDelay = 0.15f;

protected:
	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float ZoomedFOV = 45.0f;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float ZoomInterpSpeed = 15.0f;

private:
	UPROPERTY(VisibleAnywhere, Category = "WeaponSettings")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "WeaponSettings")
	USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	class USoundCue* EquipSound;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "WeaponSettings")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	EWeaponType WeaponType;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	TSubclassOf<class AShellEjector> ShellEjectorClass;

	/*
	* Ammunition
	*/
private:
	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	int32 MagCapacity;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo, Category = "WeaponSettings")
	int32 Ammo;

	UFUNCTION()
	void OnRep_Ammo();
	void SpendRound();

public:
	void AddAmmo(int32 AmmoToAdd);

public:
	void SetWeaponState(EWeaponState State);

	FORCEINLINE USphereComponent* GetWeaponAreaSphere()	const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh()			const { return WeaponMesh; }
	FORCEINLINE USoundCue* GetEquipSound()			const { return EquipSound; }

	FORCEINLINE bool IsWeaponState(EWeaponState DesiredState) const { return WeaponState == DesiredState; }

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	FORCEINLINE float GetZoomedFOV()		const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed()	const { return ZoomInterpSpeed; }
	FORCEINLINE int32 GetAmmo()				const { return Ammo; }
	FORCEINLINE int32 GetMagCapacity()		const { return MagCapacity; }

	bool IsEmpty();
	bool IsFull();

};