// @2023 All rights reversed by Reverse-Alpha Studios


#include "Pickups/AmmunitionPickup.h"

#include "Characters/BaseCharacter.h"
#include "Components/CombatComponent.h"
void AAmmunitionPickup::OnSphereOverlap
(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (BaseCharacter)
	{
		UCombatComponent* CombatComponent = BaseCharacter->Get_CombatComponent();
		if (CombatComponent)
		{
			CombatComponent->PickupAmmunition(WeaponType, AmmunitionAmount);
		}
	}
	Destroy();
}