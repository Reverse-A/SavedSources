// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseThrowableGrenades.generated.h"

UCLASS()
class SYNTHETICA_API ABaseThrowableGrenades : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseThrowableGrenades();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

protected:
	// OnBounce 
	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	UPROPERTY(VisibleAnywhere, Category = "Thrownable Properties")
	class UCapsuleComponent* CapsuleCollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Thrownable Properties")
	class USkeletalMeshComponent* ThrowableMesh;

	UPROPERTY(VisibleAnywhere, Category = "Thrownable Properties")
	class UProjectileMovementComponent* GrenadesProjectileMovementComponent;

	//Project Outside Class Variables
	class AProjectile* ProjectileClass;


	// Project Inside Class Variables
	 
	// *Sounds variables
	UPROPERTY(EditAnywhere)
	class USoundCue* GrenadeBounceSound;

	UPROPERTY(EditAnywhere)
	USoundCue* ImpactSound;

	// *Particles || Niagara Effect variables
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	// *Section for timer events 
	void StartDestroyTimer();
	void DestroyTimerFinished();

	FTimerHandle DestroyObjectTimer;

	UPROPERTY(EditAnywhere)
	float DestroyObjectTime = 1.5f;
};
