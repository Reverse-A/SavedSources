// ©2023 Reverse-A. All rights reserved.

#include "AmmoEjector.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AAmmoEjector::AAmmoEjector()
{
	PrimaryActorTick.bCanEverTick = false;

	EjectedEmptyAmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ejected Ammunation"));
	SetRootComponent(EjectedEmptyAmmoMesh);
	EjectedEmptyAmmoMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	EjectedEmptyAmmoMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	EjectedEmptyAmmoMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	EjectedEmptyAmmoMesh->SetSimulatePhysics(true);
	EjectedEmptyAmmoMesh->SetEnableGravity(true);
	EjectedEmptyAmmoMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 10.f;

}

void AAmmoEjector::BeginPlay()
{
	Super::BeginPlay();

	EjectedEmptyAmmoMesh->OnComponentHit.AddDynamic(this, &AAmmoEjector::OnHit);
	EjectedEmptyAmmoMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
	bHasPlayedShellSound = false;
}

void AAmmoEjector::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bHasPlayedShellSound && ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
		bHasPlayedShellSound = true;

		// Delay destruction by 2 seconds
		FTimerHandle DestroyTimerHandle;
		GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AAmmoEjector::DestroyObject, 2.f, false);
	}
}
void AAmmoEjector::DestroyObject()
{
	// Destroy the object
	Destroy();
}