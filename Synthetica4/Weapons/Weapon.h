// ©2023 Reverse-A. All rights reserved.

#pragma once
 
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedWeaponTypes.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Disarmed UMETA(DisplayName = "Disarmed"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};


USTRUCT(BlueprintType)
struct FCrosshairTextures
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	class UTexture2D* CrosshairCenter;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairLeft;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairRight;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairTop;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairBottom;
};


UCLASS()
class SYNTHETICA_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	void ShowPickupWidget(bool bShowWidget);
	virtual void Fire(const FVector& HitTarget);
	void SetHUDAmmo();
	void AddAmmunation(int32 AmmunationToAdd);

	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	UPROPERTY()
	class ABaseCharacter* BaseOwnerCharacter;

	UPROPERTY()
	class ABasePlayerController* BaseOwnerController;

	UPROPERTY(EditAnywhere, Category = "Sound Properties")
	class USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bAutomatic = true;

protected:
	virtual void BeginPlay() override;


	UFUNCTION()
		virtual void OnSphereOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	UFUNCTION()
		void OnSphereEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
	);


	UPROPERTY(EditAnywhere, Category = "Weapon")
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float ZoomInterpSpeed = 20.f;



private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere)
	ERangedWeaponTypes ERangedWeaponType;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TSubclassOf<class AAmmoEjector> AmmoEjectorClass;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	FCrosshairTextures CrosshairTextures;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 Ammo;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 MagCapacity;

	int8 EmptyMagazine = 0;

	void SpendRound();

public:	
	void SetWeaponState(EWeaponState State);

	FORCEINLINE EWeaponState GetWeaponState() const { return WeaponState;}

	FORCEINLINE ERangedWeaponTypes GetRangedWeaponTypes() const { return ERangedWeaponType; }

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE FCrosshairTextures GetCrosshairTextures() const{ return CrosshairTextures; }
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomedInterpSpeed() const { return ZoomInterpSpeed; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagCapacity; }
	FORCEINLINE UAnimationAsset* GetFireAnimation() const { return FireAnimation; }

	bool IsAmmoEmpty();
	bool IsAmmoFull();

};
