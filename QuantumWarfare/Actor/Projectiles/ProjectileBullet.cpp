// @2023 All rights reversed by Reverse-Alpha Studios


#include "ProjectileBullet.h"
#include "Kismet/KismetSystemLibrary.h"

AProjectileBullet::AProjectileBullet()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AProjectileBullet::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
