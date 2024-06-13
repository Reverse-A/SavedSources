// ©2023 Reverse - A.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"
#include "Synthetica/Interfaces/HitInterface.h"
#include "Synthetica/EnumTypes/CharacterTypes.h"
#include "Synthetica/Characters/BaseCharacter.h"
#include "Synthetica/Components/AttributeComponent.h"
#include "Synthetica/Components/CombatComponent.h"
#include "Synthetica/Interfaces/InteractableCrosshairInterface.h"
#include "Kismet/GameplayStatics.h"
#include "BaseEnemyAI.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;
class UBaseAIAnimInstance;
class UPawnSensingComponent;
class USkeletalMeshComponent;
class UCombatComponent;
class UMotionWarpingComponent;

UCLASS()
class SYNTHETICA_API ABaseEnemyAI : public ACharacter, public IHitInterface, public IInteractableCrosshairInterface
{
GENERATED_BODY()

public:

	ABaseEnemyAI();

	bool bWeaponAttached;
	bool bCanMeleeAttack = true;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	ABaseCharacter* CombatCharacterClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARangeEnemyAI> RangeAIClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMeleeEnemyAI> MeleeAIClass;

	UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionWarpingComponent* MotionWarpingComponent;

	float NextPatrolTargetDelay;
	EAIState AIState;

	void AI_GeneralCalculations();
	void AI_AliveConditions();
	void AI_DeathConditions();
	void IntervalResults();

	void AI_InitialState();
	void AI_IdleState();
	void AI_IdleToPatrol();
	void AI_PatrollingState();
	void AI_PatrolToIdle();
	void AI_ChaseState();
	void AI_AlertState();
	void AI_AlertStateOver();

	// PatrolTarget || CombatTarget Functions
	bool InTargetRange(AActor* Target, double Radius);

	AActor* AI_NextPatrolTarget();
	void AI_ControlNextPatrolTarget();

	UFUNCTION(BlueprintCallable)
	void AI_MoveToTarget(AActor* Target);
	UFUNCTION(BlueprintCallable)
	void AI_StopMovement();

	// AI Combat Logic
	virtual void GetHit(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReaction(const FVector& ImpactPoint);
	void HitSituation();
	void ResetHit();
	void CombatDuration();
	void CombatEnd();

	UFUNCTION(BlueprintCallable)
	void SetWarpTarget();

	UFUNCTION(BlueprintCallable)
	void AI_Melee_AttackEnd();

	// AI Sensors Functions
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	void NoiseSense(APawn* NoiseTarget, const FVector& Location, float Volume);

	UPROPERTY(BlueprintReadOnly, Category = Combat) // Added for motionwarping usage
	AActor* CombatTarget;

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	FVector CombatTargetLocation;

	UPROPERTY()
	class AAIController* EnemyAIController;

	// AI Sensor Variables
	float SightRadiusValue = 4000.f;
	float HearingThresholdValue = 2000.f;
	float LOSHearingThresholdValue = 4000.f;
	

	// FTimers

	FTimerHandle PatrolTimerHandle;
	FTimerHandle PatrolToIdleTimerHandle;
	FTimerHandle AlertStateOverTimerHandle;

	FTimerHandle HitResetTimerHandle;
	FTimerHandle CombatTimerHandle;

	float Call_DeltaTime;

	UPROPERTY(EditAnywhere)
	float MeleeAttackRadius = 150.f;
	UPROPERTY(EditAnywhere)
	float RangeAttackRadius = 1500.f;

	const float CombatRadius = 3500.f;
	const float PatrolRadius = 150.f;


	float ChaseStartAndPatrolling_MovementSpeed = 100.f;
	float ChaseMax_MovementSpeed = 500.f;
	float AI_MaxWalkSpeed;

	const float TransitionToAlertStateDelay = 5.f;
	float TransitionToAlertStateTimer;

	bool bCombatTarget_IsDetected = false;
	bool Termination;
	bool bIsDead;
	bool bIsChasing;
	bool bHit;
	bool bHitTermination;
	bool bCombatStarted;

	bool bIsPatrolling;
	bool bIsCheckNextTargetInIdle;

	// In Progress
	bool bInMeleeAttackDistance;
	bool bInRangeAttackDistance;
	bool bInCombatDistance;
	bool bInPatrolDistance;

	bool bRangeInAttackState;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class ABaseMeleeWeapon> MeleeWeaponClass;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AWeapon> RangeWeaponClass;

	void AttachWeapon();
	void DetachWeapon();

	class ABaseMeleeWeapon* MeleeEquippedWeapon;
	ABaseMeleeWeapon* MeleeDisarmedWeapon;

	class AWeapon* RangeEquippedWeapon;
	AWeapon* RangeDisarmedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Melee AI Patroling Speeches")
	TArray<USoundCue*> PatrolSpeechSounds;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontages;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* WeaponArmDisarmMontage;

private:

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY()
	UBaseAIAnimInstance* BaseAIAnimInstance;



	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;


	
	float SetHitReactValue;
	float SetDeathValue;

public:
	// Getter 
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	FORCEINLINE float GetDeathValue() const { return SetDeathValue; }
	FORCEINLINE float GetHitReactValue() const { return SetHitReactValue; }
	FORCEINLINE bool GetHit() const { return bHit; }

	FORCEINLINE bool GetRangeAIInAttackState() const { return bRangeInAttackState; }

};