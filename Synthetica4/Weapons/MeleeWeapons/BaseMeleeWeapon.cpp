// ©2023 Reverse-A. All rights reserved.

#include "BaseMeleeWeapon.h"
#include "Components/SphereComponent.h"


ABaseMeleeWeapon::ABaseMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MeleeWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeleeWeaponMesh"));
	SetRootComponent(MeleeWeaponMesh);

	MeleeWeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MeleeWeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	MeleeWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseMeleeWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	if (AreaSphere)
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//PlayEquipSound();
}

void ABaseMeleeWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	MeleeWeaponMesh->AttachToComponent(InParent, TransformRules, InSocketName);

}

void ABaseMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseMeleeWeapon::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ABaseMeleeWeapon::OnSphereEndOverlap);
	
}

// Called every frame
void ABaseMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseMeleeWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ABaseMeleeWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

