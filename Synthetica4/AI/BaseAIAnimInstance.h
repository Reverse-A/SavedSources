// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Synthetica/AI/BaseEnemyAI.h"
#include "BaseAIAnimInstance.generated.h"

UENUM(BlueprintType)
enum class ELivingState : uint8
{
	ELS_Alive UMETA(DisplayName = "Alive"),
	ELS_Dead UMETA(DisplayName = "Dead"),

	ELS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class SYNTHETICA_API UBaseAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ABaseEnemyAI* BaseEnemyAI;

protected:

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MovementSpeed; 

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;  

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir; 

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched; 

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsHit;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bAIRangeInAttackState;

	UPROPERTY(BlueprintreadOnly, meta = (AllowPrivateAccess = "true"))
	float DeathValue;

	UPROPERTY(BlueprintreadOnly, meta = (AllowPrivateAccess = "true"))
	float HitReactValue;

public :

};