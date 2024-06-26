// ©2023 Reverse-A. All rights reserved.


#include "AmmoPickup.h"
#include "Synthetica/Characters/BaseCharacter.h"
#include "Synthetica/Components/CombatComponent.h"

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (BaseCharacter)
	{
		UCombatComponent* Combat = BaseCharacter->GetCombat();
		if (Combat)
		{
			Combat->PickupAmmunition(RangeWeaponType, AmmunationAmount);
		}
	}
	Destroy();
}
