// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Ascend/AI/Actors/Humanoid/AIHumanoidModule.h"
#include "AIRangeHumanoidModule.generated.h"

class ARangedWeapon;

UCLASS()
class ASCEND_API AAIRangeHumanoidModule : public AAIHumanoidModule
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = DeveloperProperties)
	TArray<TSubclassOf<ARangedWeapon>> RangeWeapons;

	void SpawnAndAttachRangeWeapon();

};
