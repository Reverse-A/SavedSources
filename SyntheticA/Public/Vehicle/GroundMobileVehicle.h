// @2023 All rights reversed by Reverse-Alpha Studios
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GroundMobileVehicle.generated.h"

class UArrowComponent;
class UBoxComponent;
class USkeletalMeshComponent;
class UChaosVehicleMovementComponent;

UCLASS()
class SYNTHETICA_API AGroundMobileVehicle : public APawn
{
	GENERATED_BODY()

public:
	AGroundMobileVehicle();

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
	UBoxComponent* OverlapCollision;

	/**  The main skeletal mesh associated with this Vehicle */
	/*   Optionally, static mesh can also be preferred.		 */
	UPROPERTY(Category = Vehicle, VisibleDefaultsOnly,  meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> VehicleMesh;

	UPROPERTY(EditAnywhere, Category = Vehicle)
	UArrowComponent* ExitArrow;

	/** Vehicle simulation component */
	UPROPERTY(Category = Vehicle, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChaosVehicleMovementComponent> VehicleMovementComponent;

	/*Getter And Setters*/
public:
	/* Name of the VehicleMeshComponent. */
	static FName VehicleMeshComponentName;

	/* Name of the VehicleMovementComponent. */
	static FName VehicleMovementComponentName;

	static FName VehicleOverlapCollisionComponentName;

	/** Returns Mesh subobject **/
	USkeletalMeshComponent* GetMesh() const { return VehicleMesh; }

	/* Returns VehicleMovement subobject */
	UChaosVehicleMovementComponent* GetVehicleMovement() const { return VehicleMovementComponent; }

	UBoxComponent* GetOverlapCollision() const { return OverlapCollision; }

	UArrowComponent* GetExitArrow() const { return ExitArrow; }
};
