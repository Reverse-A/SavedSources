// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AmmoEject.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AAmmoEject::AAmmoEject()
{
	PrimaryActorTick.bCanEverTick = false;


	ShellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(ShellMesh);
	ShellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ShellMesh->SetSimulatePhysics(true);
	ShellMesh->SetEnableGravity(true);
	ShellMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 10.f;

	ShellEjectionImpulse = 10.0f;
}

void AAmmoEject::BeginPlay()
{
	Super::BeginPlay();

	ShellMesh->OnComponentHit.AddDynamic(this, &AAmmoEject::OnHit);
	ShellMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);

	float DelayBeforeDestroy = FMath::RandRange(1.0f, 2.5f);
	GetWorldTimerManager().SetTimer(TimerHandle_Destroy, this, &AAmmoEject::DestroyAmmoEject, DelayBeforeDestroy, false);
}

void AAmmoEject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

	if (ShellDropSound && !bOnHit)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellDropSound, GetActorLocation());
		bOnHit = true;
	}
}

void AAmmoEject::DestroyAmmoEject()
{
	bOnHit = false;
	Destroy();
}