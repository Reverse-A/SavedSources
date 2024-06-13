// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Ascend/Actor/ActorModule.h"
#include "ExplosiveActorModule.generated.h"

class UParticleSystem;
class USoundBase;

UCLASS()
class ASCEND_API AExplosiveActorModule : public AActorModule
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

#pragma region FXs & SFx
private:
	// Particle system for explosion effect
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* ExplosionParticleSystem;

	// Sound to play during explosion
	UPROPERTY(EditDefaultsOnly)
	USoundBase* ExplosionSound;
#pragma endregion
#pragma region Explosion Parameters
	// Define explosion parameters
	// Radius of the explosion
	UPROPERTY(EditAnywhere)
	float ExplosionRadius = 2000.0f; 

	// Maximum damage at the center of explosion
	float MaximumExplosionDamage = 150.0f;
#pragma endregion
#pragma region Explosion Function
	// Function to handle the explosion
	void Explode();
#pragma endregion

};
