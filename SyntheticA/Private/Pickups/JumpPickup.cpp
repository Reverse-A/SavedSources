// @2023 All rights reversed by Reverse-Alpha Studios


#include "Pickups/JumpPickup.h"

#include "Components/AttributeComponent.h"
#include "Characters/BaseCharacter.h"
void AJumpPickup::OnSphereOverlap
(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)// void AHealthPickup::OnSphereOverlap()
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if (BaseCharacter)
	{
		UAttributeComponent* AttributeComponent = BaseCharacter->GetAttributeComponent();
		if (AttributeComponent)
		{
			AttributeComponent->JumpIncrement(JumpZVelocityIncrease, JumpIncreaseTime);
		}
	}
	Destroy();
}