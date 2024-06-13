// @2023 All rights reversed by Reverse-Alpha Studios


#include "WeaponModule.h"
#include "Components/SphereComponent.h"
#include <QuantumWarfare/Character/PlayerCharacter.h>

AWeaponModule::AWeaponModule()
{
	PrimaryActorTick.bCanEverTick = false;


	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(WeaponMesh);

	float WeaponSphereRadius = 70.0f;	
	SphereComponent->SetSphereRadius(WeaponSphereRadius);

	WeaponProperties.InitializeDefaults();
}

void AWeaponModule::WeaponStatePurchasable()
{
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AWeaponModule::WeaponStateAcquired()
{
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponModule::WeaponStateDropped()
{
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponModule::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AWeaponModule::OnSphereEndOverlap);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWeaponModule::WeaponStateDestroyed()
{
	SphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponModule::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.RemoveDynamic(this, &AWeaponModule::OnSphereEndOverlap);

	Destroy();
	Destroyed();
}

void AWeaponModule::WeaponStateSold()
{
	SphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponModule::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.RemoveDynamic(this, &AWeaponModule::OnSphereEndOverlap);

	// TODO :: Pricing System
}

void AWeaponModule::SetWeaponState(EWeaponState NewState)
{
	WeaponProperties.WeaponState = NewState;
	WeaponStatement();
}

void AWeaponModule::Dissolve()
{
	if (KatanaDissolveMaterialInstance != nullptr)
	{
		UMaterialInstanceDynamic* DynamicBodyMatInst = UMaterialInstanceDynamic::Create(KatanaDissolveMaterialInstance, this);
		GetWeaponMesh()->SetMaterial(0, DynamicBodyMatInst);
		StartDissolveTimeLine(DynamicBodyMatInst);
	}
	else if (AssaultRiffleDissolveMaterialInstance != nullptr)
	{
		UMaterialInstanceDynamic* DynamicBodyMatInst = UMaterialInstanceDynamic::Create(AssaultRiffleDissolveMaterialInstance, this);
		GetWeaponMesh()->SetMaterial(0, DynamicBodyMatInst);
		StartDissolveTimeLine(DynamicBodyMatInst);
	}
}

void AWeaponModule::WeaponStatement()
{
	switch (WeaponProperties.WeaponState)
	{
	case EWeaponState::Purchasable:
		WeaponStatePurchasable();
		break;

	case EWeaponState::Acquired:
		WeaponStateAcquired();
		break;
		
	case EWeaponState::Dropped:
		WeaponStateDropped();
		break;

	case EWeaponState::Destroyed:
		WeaponStateDestroyed();
		break;

	case EWeaponState::Sold:
		WeaponStateSold();
		break;
	}
}

void AWeaponModule::BeginPlay()
{
	Super::BeginPlay();
	WeaponStatement();
}


#pragma region Overlapping
void AWeaponModule::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		{
			PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
			if (PlayerCharacter)
			{
				PlayerCharacter->SetOverlappingWeapon(this);
			}
		}
	}
}

void AWeaponModule::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ( OtherActor != nullptr && OtherActor->IsA(APlayerCharacter::StaticClass()) && PlayerCharacter == OtherActor)
	{
		if (PlayerCharacter)
		{
			PlayerCharacter->SetOverlappingWeapon(nullptr);
			PlayerCharacter = nullptr;
		}
	}
}

#pragma endregion 
