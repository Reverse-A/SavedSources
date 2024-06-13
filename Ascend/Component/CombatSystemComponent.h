// // @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatSystemComponent.generated.h"

class ABaseCharacter;
class ABaseWeapon;
class ARangedWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCEND_API UCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatSystemComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
#pragma region Classes
private:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

	UPROPERTY()
	ABaseWeapon* BaseWeapon;

	UPROPERTY()
	ARangedWeapon* RangeWeapon;

public:
	ABaseCharacter* GetCharacterFromCombatSystem()  const { return BaseCharacter; }
	ABaseWeapon*	GetWeaponFromCombatSystem()		const { return BaseWeapon; }

	void SetBaseCharacter(ABaseCharacter* GetBaseCharacter) { BaseCharacter = GetBaseCharacter; }
	void SetBaseWeaponForCombatSystemComponent(ABaseWeapon* GetBaseCharacter)		{ BaseWeapon = GetBaseCharacter; }
#pragma endregion

#pragma region AttackSystem
private:
	bool bCanMeleeAttack = true;

public:
/*MeleeAttack*/
	void MeleeAttack(bool bMeleeAttack);
	void MeleeAttackFinished();

public:
	bool CanAttack();

private:
#pragma endregion
#pragma region Fire System
private:
public:
	FTimerHandle WeaponTimerHandle;
	float WeaponFireRate;
	bool bFire = false;

public:
/*RangeAttack*/
	void RangeAttack();

public:
	bool CanFire();

	bool CanAim();
	bool IsAiming();

//Setters
	void SetFire(bool SetFire) { bFire = SetFire; }

private:
/*FireModes*/
	/*Single Shot*/
	void HandleSingleShot();
	void SingleShotTimerFinished();
	/*Semi-Automatic*/
	void HandleSemiAutomaticFire();
	int8 SemiShotCount = 3;
	/*Automatic*/
	void HandleAutomaticFire();
	void AutomaticFireTimerFinished();
#pragma endregion
#pragma region Reload
private:
	bool bIsReloading;

protected:

public:
	void Reload();
	bool CanReload();
	void SetReloading(bool bReload) { bIsReloading = bReload; }
#pragma endregion
#pragma region Crosshair
private:
	FHitResult CrosshairTraceHitResult;
	FVector CrosshairImpactPoint;

	void PerformCrosshairTrace(FHitResult& TraceHitResult);
public:

#pragma endregion
};
