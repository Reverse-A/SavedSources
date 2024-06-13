// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|


#include "Weapon/ShellEjector.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AShellEjector::AShellEjector()
{
	PrimaryActorTick.bCanEverTick = false;

	EjectedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(EjectedMesh);
	EjectedMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	EjectedMesh->SetSimulatePhysics(true);
	EjectedMesh->SetEnableGravity(true);
	EjectedMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 10.f;

	ShellEjectionImpulse = 10.0f;
}

void AShellEjector::BeginPlay()
{
	Super::BeginPlay();

	EjectedMesh->OnComponentHit.AddDynamic(this, &AShellEjector::OnHit);
	EjectedMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
}

void AShellEjector::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ShellDropSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellDropSound, GetActorLocation());
	}
	Destroy();
}
