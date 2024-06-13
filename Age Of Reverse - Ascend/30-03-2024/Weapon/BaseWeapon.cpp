// @2023 All rights reversed by Reverse-Alpha Studios

#include "BaseWeapon.h"

#include "Ascend/Character/ShortDebugMacros.h"
#include "Ascend/Character/BaseCharacter.h"
#include "Ascend/Component/CombatSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


static FName NAME_AreaCollision(TEXT("Area Collision"));
static FName NAME_WeaponMesh(TEXT("WeaponMesh"));

#pragma region Fundementals
ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(NAME_WeaponMesh); 
	check(WeaponMesh)

	AreaCollision = CreateDefaultSubobject<USphereComponent>(NAME_AreaCollision);
	check(AreaCollision);

	SetRootComponent(WeaponMesh);
	AreaCollision->SetupAttachment(WeaponMesh);
	AreaCollision->SetSphereRadius(100.0f);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (GetObtainingMethod() == EObtainingMethod::Pickable)
	{
		SetWeaponState(EWeaponState::EWS_Initial);
	}
	else if (GetObtainingMethod() == EObtainingMethod::Purchasable)
	{
		SetWeaponState(EWeaponState::EWS_Tradeable);
	}
}
#pragma endregion
#pragma region WeaponState
void ABaseWeapon::CheckWeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Initial:
		InitialState();
		break;

	case EWeaponState::EWS_Equipped:
		EquipState();
		break;

	case EWeaponState::EWS_Dropped:
		DropState();
		break;

	case EWeaponState::EWS_Tradeable:
		TradeableState();
		break;
	}
}
void ABaseWeapon::InitialState()
{
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	AreaCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnSphereOverlap);
	AreaCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnSphereEndOverlap);
}
void ABaseWeapon::EquipState()
{
	AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	if (AreaCollision->OnComponentBeginOverlap.IsBound())
	{
		AreaCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ABaseWeapon::OnSphereOverlap);
	}

	if (AreaCollision->OnComponentEndOverlap.IsBound())
	{
		AreaCollision->OnComponentEndOverlap.RemoveDynamic(this, &ABaseWeapon::OnSphereEndOverlap);
	}
	PlayEquipWeaponSound();
}
void ABaseWeapon::DropState()
{
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	AreaCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnSphereOverlap);
	AreaCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnSphereEndOverlap);

	// TODO : drop weapon sound
}
void ABaseWeapon::TradeableState()
{
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (AreaCollision->OnComponentBeginOverlap.IsBound())
	{
		AreaCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ABaseWeapon::OnSphereOverlap);
	}

	if (AreaCollision->OnComponentEndOverlap.IsBound())
	{
		AreaCollision->OnComponentEndOverlap.RemoveDynamic(this, &ABaseWeapon::OnSphereEndOverlap);
	}
}
#pragma endregion
#pragma region Overlap
void ABaseWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BaseCharacter = Cast<ABaseCharacter>(OtherActor);

	if (BaseCharacter)
	{
		BaseCharacter->SetOverlapWeapon(this);
	}
}

void ABaseWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	BaseCharacter = Cast<ABaseCharacter>(OtherActor);

	if (BaseCharacter)
	{
		BaseCharacter->SetOverlapWeapon(nullptr);
	}
}
#pragma endregion
#pragma region Sounds
void ABaseWeapon::PlayEquipWeaponSound()
{
	if (EquipSound != nullptr)
	{
		UGameplayStatics::PlaySound2D
		(
			GetWorld(),
			EquipSound
		);
	}
	else
	{
		LOG_TO_DESKTOP("Do not forget to add 'EquipSound'");
	}
}
void ABaseWeapon::InsertAndPlaySoundAtLocation(USoundCue* InsertPlayableSoundAtLocation, FVector Location)
{
	if (InsertPlayableSoundAtLocation != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			this,
			InsertPlayableSoundAtLocation,
			Location
		);
	}
}
void ABaseWeapon::InsertAndPlaySound2D(USoundCue* InsertPlayableSound2D)
{
	if (InsertPlayableSound2D != nullptr)
	{
		UGameplayStatics::PlaySound2D
		(
			GetWorld(),
			InsertPlayableSound2D
		);
	}
}
#pragma endregion