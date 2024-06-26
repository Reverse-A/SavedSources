// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "RaycastWeapons.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API AShotgun : public ARaycastWeapons
{
	GENERATED_BODY()
public:

	virtual void Fire(const FVector& HitTarget) override;

private:
	UPROPERTY(EditAnywhere, Category = "Bullet Spread")
	uint32 NumberOfPellets = 10;

};
