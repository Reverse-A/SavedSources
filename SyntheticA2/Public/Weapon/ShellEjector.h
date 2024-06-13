// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShellEjector.generated.h"

UCLASS()
class SYNTHETICA_API AShellEjector : public AActor
{
	GENERATED_BODY()
	
public:
	AShellEjector();

	virtual void BeginPlay() override;
protected:
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* EjectedMesh;

	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse;

	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	class USoundCue* ShellDropSound;

};
