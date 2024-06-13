// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AIDefaultAnimInstance.generated.h"

class AAIEntityModule;
UCLASS()
class ASCEND_API UAIDefaultAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY()
	AAIEntityModule* AIEntity;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float PawnSpeed = 0.0f;
};
