// ©2023 Reverse-A. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMeleeWeapon.generated.h"

UCLASS()
class SYNTHETICA_API ABaseMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseMeleeWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;



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
private:

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USkeletalMeshComponent* MeleeWeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

public:
	FORCEINLINE USkeletalMeshComponent* GetMeleeWeaponMesh() const { return MeleeWeaponMesh; }
};
