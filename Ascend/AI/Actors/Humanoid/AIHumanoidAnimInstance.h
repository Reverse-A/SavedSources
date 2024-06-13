// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AIHumanoidAnimInstance.generated.h"

class AMeleeWeapon;
class ARangedWeapon;
class AAIEntityModule;

UCLASS()
class ASCEND_API UAIHumanoidAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private:
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AIMovementSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bEquippedWeapon = false;

	UPROPERTY()
	AAIEntityModule* AIEntity = nullptr;

	UPROPERTY()
	AMeleeWeapon* MeleeWeaponClass = nullptr;

	UPROPERTY()
	ARangedWeapon* RangedWeaponClass = nullptr;

public:
	float GetHumanoidSpeed() const { return AIMovementSpeed; }
	bool GetEquippedWeapon() const { return bEquippedWeapon; }

	void SetHumanoidSpeed(float Speed) { AIMovementSpeed = Speed; }
	void SetEquippedWeapon(bool bEquipped) { bEquippedWeapon = bEquipped; }

	void SetMeleeWeaponClass (AMeleeWeapon* NewMeleeWeaponClass)   { MeleeWeaponClass  = NewMeleeWeaponClass; }
	void SetRangedWeaponClass(ARangedWeapon* NewRangedWeaponClass) { RangedWeaponClass = NewRangedWeaponClass; }


};
