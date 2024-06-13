// // @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Ascend/Weapon/WeaponStructureBase.h"
#include "CharacterDefaultAnimInstance.generated.h"

class ABaseCharacter;

UCLASS()
class ASCEND_API UCharacterDefaultAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

/*Structs*/
protected:
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	EWeaponState WeaponState;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	EMeleeWeaponType MeleeWeaponType;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	ERangedWeaponType RangedWeaponType;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CharacterSpeed		= 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsFalling				= false;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking			= false;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsReloading = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming				= false;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FVector Velocity			= FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FVector Direction			= FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RotateBonePitch		= 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RotateBoneYaw			= 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CharacterForwardSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CharacterRightSpeed	= 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LeanRight				= 0.0f;

public:
	void SetWeaponType(EWeaponType NewWeaponType) { WeaponType = NewWeaponType; }
	void SetMeleeWeaponType(EMeleeWeaponType NewMeleeWeaponType) { MeleeWeaponType = NewMeleeWeaponType; }
	void SetRangedWeaponType(ERangedWeaponType NewRangedWeaponType) { RangedWeaponType = NewRangedWeaponType; }

	/*This function set the character's attacking animations based on a boolean parameter.*/
	FORCEINLINE void SetIsAttacking(bool bAttacking) { bIsAttacking = bAttacking; }
	FORCEINLINE void SetIsAiming(bool bAiming) { bIsAiming = bAiming; }


	/*AnimationNotifies*/
protected:
	UFUNCTION(BlueprintCallable, Category = "AnimationNotifiers")
	void FinishAttackNotify()
	{
		bIsAttacking = false;
	}

	UFUNCTION(BlueprintCallable, Category = "AnimationNotifiers")
	void FinishReloading();

};
