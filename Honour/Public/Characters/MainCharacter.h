// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "MainCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class ASoul;
class ATreasure;
class UAnimMontage;
class UMainOverlay;

UCLASS()
class HONOUR_API AMainCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	AMainCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(class AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;

	UCharacterMovementComponent* CharacterMovement;

protected:
	virtual void BeginPlay() override;

	// Callbacks for input
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EKeyPressed();
	void Dodge();

	/*<Combats>*/
	void EquipWeapon(AWeapon* Weapon);
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	virtual void Die_Implementation() override;
	bool HasEnoughStamina();
	bool IsOccupied();
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();
	
	void PlayEquipMontage(const FName& SectionName);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	void SpeedUp(float Value);
	bool idleState;

private:
	bool IsUnoccupied();

	void InitializeMainOverlay();
	void SetHUDHealth();

	/* <Character Components*/
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	/*<States>*/
	ECharacterEquippedState CharacterEquippedState = ECharacterEquippedState::ECES_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	UMainOverlay* MainOverlay;

public:
	FORCEINLINE ECharacterEquippedState GetCharacterEquippedState() const { return CharacterEquippedState;}
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
