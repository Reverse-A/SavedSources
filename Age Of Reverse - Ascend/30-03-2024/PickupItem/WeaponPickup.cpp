// // @2023 All rights reversed by Reverse-Alpha Studios


#include "WeaponPickup.h"

AWeaponPickup::AWeaponPickup()
{
	PickupItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	PickupItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	PickupItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}