// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ASCEND_API Constants
{
public:
	Constants();
	~Constants();

private:
	FVector const HealthBarZ = FVector(0.0f, 0.0f, 105.f);

public:
	FVector GetHealthBarZ() const { return HealthBarZ; }
};
