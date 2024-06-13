// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoEject.generated.h"

UCLASS()
class SYNTHETICA_API AAmmoEject : public AActor
{
	GENERATED_BODY()
	
public:	
	AAmmoEject();

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ShellMesh;

	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	class USoundCue* ShellDropSound;

	bool bOnHit = false;

/*Destroying The Actor*/
	FTimerHandle TimerHandle_Destroy;
	void DestroyAmmoEject();
};
