// @2023 All rights reversed by Reverse-Alpha Studios


#include "ExperiencePointPickup.h"
#include "Ascend/Character/BaseCharacter.h"
#include "Ascend/Component/LevelSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AExperiencePointPickup::AExperiencePointPickup()
{

}

void AExperiencePointPickup::BeginPlay()
{
	Super::BeginPlay();

	if (ExperienceParticles)
	{
		// Get the location where you want to spawn the Niagara-system
		FVector SpawnLocation = GetActorLocation();

		// Optionally, you can specify a rotation and scale for the spawned Niagara system
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FVector SpawnScale = FVector(1.0f);

		// Spawn the Niagara system
		NiagaraComponent = 
		UNiagaraFunctionLibrary::SpawnSystemAtLocation
		(
			GetWorld(),
			ExperienceParticles,
			SpawnLocation, 
			SpawnRotation, SpawnScale,
			false, 
			true, 
			ENCPoolMethod::None,
			true
		);
	}
	else
	{
	}
}

void AExperiencePointPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ABaseCharacter>())
	{
		PlayerCharacter = Cast<ABaseCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			float Experience = PlayerCharacter->GetLevelSystem()->GetLevelStructure().GetAndCalculateExperiencePoint();
			PlayerCharacter->GetLevelSystem()->IncreaseExperience(Experience);
			PlayerCharacter->UpdateHUDExperience();

			if (NiagaraComponent)
			{
				// Deactivate the Niagara system
				if (NiagaraComponent->IsActive())
				{
					NiagaraComponent->DeactivateImmediate();
				}

				// Destroy the Niagara component
				NiagaraComponent->DestroyComponent();
				NiagaraComponent = nullptr; // Set to nullptr to avoid potential access is

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
				{
					Destroy();
				}, 0.1f, false); // Adjust the delay as needed (0.1 second in this example)
			}
			else
			{
				// If NiagaraComponent is null, destroy this actor immediately
				Destroy();
			}
		}
	}
}

void AExperiencePointPickup::Destroyed()
{
	Super::Destroyed();

	// Destroy the Niagara component if it exists
	if (NiagaraComponent)
	{
		// Deactivate the Niagara system
		if (NiagaraComponent->IsActive())
		{
			NiagaraComponent->DeactivateImmediate();
		}
		// Destroy the Niagara component
		NiagaraComponent->DestroyComponent();
	}
}