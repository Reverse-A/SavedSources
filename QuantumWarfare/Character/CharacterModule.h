// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include <QuantumWarfare/Components/ArsenalComponent.h>
#include <QuantumWarfare/Interaction/CombatInterface.h>
#include <QuantumWarfare/Actor/Weapons/Range/RangeWeaponInterface.h>

#include "CharacterModule.generated.h"

class UAbilitySystem;
class UAbilitySystemComponent;
class UAttributeSet;
class UAttributeSystem;
class UAnimMontage;
class UGameplayAbility;
class UGameplayEffect;
class UPCharacterAnimInstance;
class UStaticMeshComponent;

UCLASS(Abstract)
class QUANTUMWARFARE_API ACharacterModule : public ACharacter,
	public IAbilitySystemInterface,
	public ICombatInterface, 
	public IRangeWeaponInterface
{
	GENERATED_BODY()

public:
	ACharacterModule();

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	virtual void PreInitializeComponents() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	 
protected:
	UPROPERTY()
	UPCharacterAnimInstance* CharacterAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arsenal", meta = (AllowPrivateAccess = "true"))
	UArsenalComponent* ArsenalComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();

private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

#pragma region AnimationProps
public:
	void ABPSetWeaponEquipped();
#pragma endregion

#pragma region Hit-Death Cond.
public:
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual void Death() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_Death();
private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

/*Dissolve*/
protected:
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeLine
	(
		UMaterialInstanceDynamic* BodyDynamicMaterialInstance,
		UMaterialInstanceDynamic* HairDynamicMaterialInstance,
		UMaterialInstanceDynamic* HeadDynamicMaterialInstance
	);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> BodyDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> HairDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> HeadDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
#pragma endregion
};
