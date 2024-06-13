// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class SYNTHETICA_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();


public:
	virtual void Destroyed() override;
protected:
	virtual void BeginPlay() override;

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

private:	
	UPROPERTY(EditAnywhere, Category = "ClassProperties")
	class USphereComponent* OverlapSphere;

	UPROPERTY(EditAnywhere, Category = "ClassProperties")
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, Category = "ClassProperties")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, Category = "ClassProperties")
	class UNiagaraComponent* PickupVisualComponent;

	UPROPERTY(EditAnywhere, Category = "ClassProperties")
	class UNiagaraSystem* PickupEffect;
	
	FTimerHandle BindOverlapTimer;

	float BindOverlapTime = 0.25f;

	void BindOverlapTimerFinished();

public:
	FORCEINLINE USoundCue* GetPickupSound() const {	return PickupSound;	}
	FORCEINLINE USphereComponent* GetOverlapSphere() const { return OverlapSphere; }

public:
	FORCEINLINE UStaticMeshComponent* GetPickupMesh() const { return PickupMesh; }
};