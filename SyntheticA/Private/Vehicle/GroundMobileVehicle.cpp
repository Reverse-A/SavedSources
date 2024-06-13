// @2023 All rights reversed by Reverse-Alpha Studios
#include "Vehicle/GroundMobileVehicle.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"


FName AGroundMobileVehicle::VehicleMeshComponentName(TEXT("VehicleMesh"));
FName AGroundMobileVehicle::VehicleMovementComponentName(TEXT("VehicleMovementComponent"));
FName AGroundMobileVehicle::VehicleOverlapCollisionComponentName(TEXT("VehicleOverlapComponent"));

AGroundMobileVehicle::AGroundMobileVehicle()
{
	OverlapCollision = CreateDefaultSubobject<UBoxComponent>(VehicleOverlapCollisionComponentName);
	OverlapCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	VehicleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(VehicleMeshComponentName);
	VehicleMesh->SetCollisionProfileName(UCollisionProfile::Vehicle_ProfileName);
	VehicleMesh->BodyInstance.bSimulatePhysics			= false;
	VehicleMesh->BodyInstance.bNotifyRigidBodyCollision = true;
	VehicleMesh->BodyInstance.bUseCCD					= true;
	VehicleMesh->SetGenerateOverlapEvents(true);
	VehicleMesh->SetCanEverAffectNavigation(false);
	VehicleMesh->SetSimulatePhysics(true);
	RootComponent = VehicleMesh;

	OverlapCollision->SetupAttachment(VehicleMesh);

	VehicleMovementComponent = CreateDefaultSubobject<UChaosVehicleMovementComponent, UChaosWheeledVehicleMovementComponent>(VehicleMovementComponentName);
	VehicleMovementComponent->SetIsReplicated(true); // Enable replication by default
	VehicleMovementComponent->UpdatedComponent = VehicleMesh;

	ExitArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ExitArrow"));
	ExitArrow->SetupAttachment(VehicleMesh);
	ExitArrow->SetRelativeLocation(FVector(0.0f, -150.0f, 0.0f));
	ExitArrow->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}

void AGroundMobileVehicle::BeginPlay()
{
	Super::BeginPlay();

	if (OverlapCollision != nullptr)
	{
		OverlapCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		OverlapCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}
