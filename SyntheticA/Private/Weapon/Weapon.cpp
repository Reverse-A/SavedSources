// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

#include "Animation/AnimationAsset.h"
#include "Characters/BaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerController/PlayerControllerManager.h"
#include "Weapon/AmmoEject.h"

#include "Net/UnrealNetwork.h"
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates				= true;
	bNetUseOwnerRelevancy	= true;
	bAlwaysRelevant			= false;
	SetReplicateMovement(true);


	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>	(TEXT("WeaponMesh"));
	AreaSphere = CreateDefaultSubobject<USphereComponent>		(TEXT("AreaSphere"));

	SetRootComponent(WeaponMesh);
	AreaSphere->SetupAttachment(RootComponent);

	AreaSphere->SetSphereRadius(60.0f);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

#pragma region ClassProps
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion

#pragma region NetWorking
void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
	DOREPLIFETIME_CONDITION(AWeapon, bUseServerSideRewind, COND_OwnerOnly);
}

#pragma endregion

#pragma region Overlapping
void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);

	if (BaseCharacter)
	{
		BaseCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (BaseCharacter)
	{
		BaseCharacter->SetOverlappingWeapon(nullptr);

		PlayerOwnerController = PlayerOwnerController == nullptr ? Cast<APlayerControllerManager>(BaseCharacter->Controller) : PlayerOwnerController;

		if (PlayerOwnerController)
		{
			PlayerOwnerController->SetHUDWeaponType(this->Get_WeaponType(), false);
		}
	}
}
#pragma endregion

#pragma region FireSystem
void AWeapon::Fire(const FVector& HitTarget)
{

	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}

	if (AmmoEjectClass)
	{
		const USkeletalMeshSocket* AmmoEjectSocket = WeaponMesh->GetSocketByName(FName("AmmoEject"));

		if (AmmoEjectSocket)
		{
			FTransform EjectSocketTransform = AmmoEjectSocket->GetSocketTransform(WeaponMesh);

			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<AAmmoEject>
				(
					AmmoEjectClass,
					EjectSocketTransform.GetLocation(),
					EjectSocketTransform.GetRotation().Rotator()
				);
			}
		}
	}
	SpendRound();
}

FVector AWeapon::TraceEndWithScatter(const FVector& HitTarget)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket == nullptr)
	{
		return FVector();
	}

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector TraceStart		 = SocketTransform.GetLocation();


	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter		 = TraceStart + ToTargetNormalized * DistanceToSphere;
	const FVector RandomVector		 = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
	const FVector EndLocation		 = SphereCenter + RandomVector;
	const FVector ToEndLocation		 = EndLocation - TraceStart;

	return FVector(TraceStart + ToEndLocation * TRACE_MAX_LENGHT / ToEndLocation.Size());
}
#pragma endregion

#pragma region WeaponState
void AWeapon::OnWeaponEquippedPrimary()
{

	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	if (WeaponType == EWeaponType::EWT_SubmachineGun)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}

	BaseOwnerCharacter = BaseOwnerCharacter == nullptr ? Cast<ABaseCharacter>(GetOwner()) : BaseOwnerCharacter;
	if (BaseOwnerCharacter && bUseServerSideRewind)
	{
		PlayerOwnerController = PlayerOwnerController == nullptr ? Cast<APlayerControllerManager>(BaseOwnerCharacter->Controller) : PlayerOwnerController;
		if (PlayerOwnerController && HasAuthority() && !PlayerOwnerController->HighPingDelegate.IsBound())
		{
			PlayerOwnerController->HighPingDelegate.AddDynamic(this, &AWeapon::OnPingTooHigh);
		}
	}
}

void AWeapon::OnWeaponEquippedSecondary()
{
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	if (WeaponType == EWeaponType::EWT_SubmachineGun)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}


	BaseOwnerCharacter = BaseOwnerCharacter == nullptr ? Cast<ABaseCharacter>(GetOwner()) : BaseOwnerCharacter;
	if (BaseOwnerCharacter && bUseServerSideRewind)
	{
		PlayerOwnerController = PlayerOwnerController == nullptr ? Cast<APlayerControllerManager>(BaseOwnerCharacter->Controller) : PlayerOwnerController;
		if (PlayerOwnerController && HasAuthority() && PlayerOwnerController->HighPingDelegate.IsBound())
		{
			PlayerOwnerController->HighPingDelegate.RemoveDynamic(this, &AWeapon::OnPingTooHigh);
		}
	}
}

void AWeapon::OnWeaponDropped()
{
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,   ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	BaseOwnerCharacter = BaseOwnerCharacter == nullptr ? Cast<ABaseCharacter>(GetOwner()) : BaseOwnerCharacter;
	if (BaseOwnerCharacter && bUseServerSideRewind)
	{
		PlayerOwnerController = PlayerOwnerController == nullptr ? Cast<APlayerControllerManager>(BaseOwnerCharacter->Controller) : PlayerOwnerController;
		if (PlayerOwnerController && HasAuthority() && PlayerOwnerController->HighPingDelegate.IsBound())
		{
			PlayerOwnerController->HighPingDelegate.RemoveDynamic(this, &AWeapon::OnPingTooHigh);
		}
	}
}

void AWeapon::OnWeaponStateSet()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_PrimaryEquipped:
		OnWeaponEquippedPrimary();
		break;
	case EWeaponState::EWS_SecondaryEquipped:
		OnWeaponEquippedSecondary();
		break;
	case EWeaponState::EWS_Dropped:
		OnWeaponDropped();
		break;
	}
}

void AWeapon::OnRep_WeaponState()
{
	OnWeaponStateSet();
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	OnWeaponStateSet();
}

#pragma endregion

#pragma region Ammunition-Props
void AWeapon::SetHUDAmmo()
{
	BaseOwnerCharacter = BaseOwnerCharacter == nullptr ? Cast<ABaseCharacter>(GetOwner()) : BaseOwnerCharacter;
	if (BaseOwnerCharacter)
	{
		PlayerOwnerController = PlayerOwnerController == nullptr ? Cast<APlayerControllerManager>(BaseOwnerCharacter->Controller) : PlayerOwnerController;
		if (PlayerOwnerController)
		{
			PlayerOwnerController->SetHUDWeaponAmmo(Ammo);
		}
	}
}

void AWeapon::SpendRound()
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MagazineCapacity);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController && PlayerController->HasAuthority())
	{
		SetHUDAmmo();
	}
	
	if (HasAuthority())
	{
		Client_UpdateAmmo(Ammo);
	}
	else
	{
		Sequence++;
	}
}

void AWeapon::Client_UpdateAmmo_Implementation(int32 ServerAmmo)
{
	if (HasAuthority())
	{
		return;
	}

	Ammo = ServerAmmo;
	Sequence--;
	Ammo -= Sequence;
	SetHUDAmmo();
}

void AWeapon::AddAmmo(int32 AmmoToAdd)
{
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagazineCapacity);
	SetHUDAmmo();
	Client_AddAmmo(AmmoToAdd);
}

void AWeapon::Client_AddAmmo_Implementation(int32 AmmoToAdd)
{
	if (HasAuthority())	return;

	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagazineCapacity);
	BaseOwnerCharacter = BaseOwnerCharacter == nullptr ? Cast<ABaseCharacter>(GetOwner()) : BaseOwnerCharacter;
	if (BaseOwnerCharacter && BaseOwnerCharacter->Get_CombatComponent() && IsFull())
	{
		BaseOwnerCharacter->Get_CombatComponent()->SetReload(false);
	}
	SetHUDAmmo();
}


void AWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();
	if (Owner == nullptr)
	{
		BaseOwnerCharacter	  = nullptr;
		PlayerOwnerController = nullptr;
	}
	else
	{
		BaseOwnerCharacter = BaseOwnerCharacter == nullptr ? Cast<ABaseCharacter>(Owner) : BaseOwnerCharacter;
		if (BaseOwnerCharacter && BaseOwnerCharacter->GetEquippedWeapon() && BaseOwnerCharacter->GetEquippedWeapon() == this)
		{
			SetHUDAmmo();
		}
	}
}
#pragma endregion

#pragma region Equip&Drop-System
void AWeapon::Drop_Weapon()
{
	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	WeaponMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	BaseOwnerCharacter	= nullptr;
	PlayerOwnerController = nullptr;
}
#pragma endregion


#pragma region Ping
void AWeapon::OnPingTooHigh(bool bPingTooHigh)
{
	bUseServerSideRewind = !bPingTooHigh;
}
#pragma endregion