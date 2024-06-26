// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Synthetica/AI/BaseEnemyAI.h"
#include "RangeEnemyAI.generated.h"


UCLASS()
class SYNTHETICA_API ARangeEnemyAI : public ABaseEnemyAI
{
	GENERATED_BODY()
public:


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	class AWeapon* WeaponClass;

	void LookAtTarget();
	void StartFireTimer_AIRange();
	void Fire_AIRange();


	FTimerHandle AIRange_FireTimer;
	bool bCanRangeFire = true;

	float minFireDelay = 0.1f;
	float maxFireDelay = 2.5f;

private:
	void AI_Range_GeneralConditions(float DeltaTime);
	void AI_Range_IdleConditions();
	void AI_Range_PatrolConditions(float DeltaTime);
	void AI_Range_AlertState();
	void AI_Range_AttackState();

	
};