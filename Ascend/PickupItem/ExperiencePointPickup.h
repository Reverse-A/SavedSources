// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "PickupItemManager.h"
#include "ExperiencePointPickup.generated.h"

class ABaseCharacter;

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ASCEND_API AExperiencePointPickup : public APickupItemManager
{
	GENERATED_BODY()
	
public:
	AExperiencePointPickup();

private:
	UPROPERTY()
	ABaseCharacter* PlayerCharacter = nullptr;
	 
protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	virtual void OnSphereOverlap
	(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;


protected:
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditAnywhere, Category = "Developer Settings")
	UNiagaraSystem* ExperienceParticles;


};
