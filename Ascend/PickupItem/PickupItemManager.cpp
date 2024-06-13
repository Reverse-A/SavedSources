// // @2023 All rights reversed by Reverse-Alpha Studios


#include "PickupItemManager.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

//static FName NAME_AreaCollision(TEXT("AreaCollision"));
static FName NAME_StaticMesh(TEXT("PickupMesh"));

APickupItemManager::APickupItemManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;


	PickupItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(NAME_StaticMesh);	check(PickupItemMesh);
	SetRootComponent(PickupItemMesh);
	PickupItemMesh->SetSimulatePhysics(true);

	AreaCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AreaCollision")); check(AreaCollision);
	
	AreaCollision->SetupAttachment(PickupItemMesh);
	AreaCollision->SetSphereRadius(100.0f);

	// Bind overlap events
	AreaCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupItemManager::OnSphereOverlap);
	AreaCollision->OnComponentEndOverlap.AddDynamic(this,	&APickupItemManager::OnSphereEndOverlap);
}

void APickupItemManager::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
	{
		return;
	}
}

void APickupItemManager::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

