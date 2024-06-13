// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "Synthetica/AI/BaseEnemyAI.h"
#include "MeleeEnemyAI.generated.h"


UCLASS()
class SYNTHETICA_API AMeleeEnemyAI : public ABaseEnemyAI
{
	GENERATED_BODY()

public:
	void MeleeAttack();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void StateConditions(float DeltaTime);

	void AI_Melee_AttackState();
	void AI_Melee_IdleConditions();
	void AI_Melee_PatrolConditions(float DeltaTime);
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void PlayRandomPatrolSpeech();


	float InMeleeAttackRadiusTimer;
	float PatrolSpeechRandomTimerDelay;
	float DodgeTimer;
	bool bPatrolSpeechIsTriggered = false;


};