// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "QWAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class QUANTUMWARFARE_API UQWAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UQWAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
