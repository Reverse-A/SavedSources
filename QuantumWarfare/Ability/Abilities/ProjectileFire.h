// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "DamageGameplayAbility.h"
#include "ProjectileFire.generated.h"

class AProjectileModule;
class AProjectileRocket;

class UGameplayEffect;

UCLASS()
class QUANTUMWARFARE_API UProjectileFire : public UDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility
	(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileModule> ProjectileClass;


	//TODO: Projectile Rocket 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileRocket> ProjectileRocketClass;
};