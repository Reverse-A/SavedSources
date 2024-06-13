// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "CharacterModule.h"
#include <QuantumWarfare/Interaction/EnemyInterface.h>
#include <QuantumWarfare/UI/WidgetController/PlayerOverlayWidgetController.h>
#include <QuantumWarfare/Ability/Data/CharacterClassInformation.h>
#include "EnemyCharacter.generated.h"

class UAnimMontage;
class UWidgetComponent;
class UBehaviorTree;
class AAIControllerModule;

UCLASS()
class QUANTUMWARFARE_API AEnemyCharacter : public ACharacterModule, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	virtual int32 GetPlayerLevel() override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Vanguard;

#pragma region Widgets
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBar;
#pragma endregion

#pragma region CombatMechanics
public:
	UPROPERTY(BlueprintReadOnly, Category = "CombatMechanics")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "CombatMechanics")
	float BaseWalkSpeed = 300.f;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
#pragma endregion

#pragma region Hit-Death Condi.
protected:
	virtual void Death() override;
	UPROPERTY(BlueprintReadOnly)
	bool bDeath = false;
#pragma endregion


#pragma region BehaviorTree
private:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAIControllerModule> AIControllerModule;
#pragma endregion

};
