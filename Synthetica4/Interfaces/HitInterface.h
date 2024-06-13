// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SYNTHETICA_API IHitInterface
{
	GENERATED_BODY()

public:
	virtual void GetHit(const FVector& ImpactPoint) = 0;
};
