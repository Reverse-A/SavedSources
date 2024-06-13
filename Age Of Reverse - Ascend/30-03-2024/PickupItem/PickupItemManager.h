// // @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItemManager.generated.h"

class USphereComponent;
class UStaticMeshComponent;
UCLASS()
class ASCEND_API APickupItemManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupItemManager();

protected:
	UPROPERTY(VisibleAnywhere, Category = DeveloperProperties)
	USphereComponent* AreaCollision;

	UPROPERTY(VisibleAnywhere, Category = DeveloperProperties)
	UStaticMeshComponent* PickupItemMesh;

private:
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function to handle actor end overlap events
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
