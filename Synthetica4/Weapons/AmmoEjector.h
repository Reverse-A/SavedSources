// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoEjector.generated.h"

UCLASS()
class SYNTHETICA_API AAmmoEjector : public AActor
{
	GENERATED_BODY()
	
public:	
	AAmmoEjector();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DestroyObject();

private:	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* EjectedEmptyAmmoMesh;

	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse;

	UPROPERTY(EditAnywhere)
	class USoundCue* ShellSound;

	bool bHasPlayedShellSound;
};
