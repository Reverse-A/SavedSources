// ©2023 Reverse - A Company.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"

#include "CustomPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (CustomCamera), meta = (BlueprintSpawnableComponent))
class SYNTHETICA_API ACustomPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ACustomPlayerCameraManager();

	class ABaseCharacter* BaseCharacter;

// CrouchCameraView
public:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float CrouchBlendDuration = 0.5f;

	float CrouchBlendTime;
	
public:

};
