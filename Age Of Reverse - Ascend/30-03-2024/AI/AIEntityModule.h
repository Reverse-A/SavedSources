// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ascend/Interface/CombatInterface.h"
#include "AIEntityModule.generated.h"

#pragma region Pre-Called
class AAIPatrolPath;

class UAnimMontage;
class UAttributeComponent;
class UAIDefaultAnimInstance;
class UBehaviorTree;
class UHealthBar;
class UWidgetComponent;
#pragma endregion
UCLASS()
class ASCEND_API AAIEntityModule : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAIEntityModule();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

#pragma region MainComps
protected:
	USkeletalMeshComponent* AIMesh;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* AICapsuleComponent;


	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* AttributeComponent;

#pragma endregion
#pragma region Combat-System
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DeveloperProperties, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	int MeleeAttack_Implementation() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DeveloperProperties, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DrawWeaponMontage;

public:
	UAnimMontage* GetAttackMontage() const { return AttackMontage; }

	UAnimMontage* GetDrawWeaponMontage() const { return DrawWeaponMontage; }

#pragma endregion

#pragma region AIProps
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DeveloperProperties, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DeveloperProperties, meta = (AllowPrivateAccess = "true"))
	AAIPatrolPath* AIPatrolPath;

public:
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	AAIPatrolPath* GetAIPatrolPath() const { return AIPatrolPath; }
#pragma endregion
#pragma region Life-Conditions
private:
	void Death();

protected:
	virtual void OnDeath();

public:
	bool IsAIDead();
#pragma endregion
#pragma region Damage-System
private:
	float DamageAmount;

public:
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void ReceiveDamage(float DamageValue);
#pragma endregion
#pragma region UI/Widgets
private:
	UPROPERTY(EditAnywhere, Category = DeveloperProperties)
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY()
	UHealthBar* HealthBarWidget;
#pragma endregion
};
