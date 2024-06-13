// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Ascend/AI/AIEntityModule.h"

#include "Ascend/Weapon/MeleeWeapon.h"
#include "Ascend/Weapon/RangedWeapon.h"

#include "AIHumanoidModule.generated.h"

class ARangedWeapon;
class AMeleeWeapon;

class UAIHumanoidAnimInstance;

UCLASS()
class ASCEND_API AAIHumanoidModule : public AAIEntityModule
{
	GENERATED_BODY()
	
public:
	AAIHumanoidModule();
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UAIHumanoidAnimInstance* AIHumanoidAnimInstance;

	UPROPERTY()
	ARangedWeapon* RangedWeaponClass = nullptr;

	UPROPERTY()
	AMeleeWeapon* MeleeWeaponClass = nullptr;

public:
	FORCEINLINE UAIHumanoidAnimInstance* GetAIHumanoidAnimInstance() const { return AIHumanoidAnimInstance; }

	void SetMeleeWeaponClass(AMeleeWeapon* NewMeleeWeaponClass) { MeleeWeaponClass = NewMeleeWeaponClass; }
	void SetRangedWeaponClass(ARangedWeapon* NewRangedWeaponClass) { RangedWeaponClass = NewRangedWeaponClass; }

};
