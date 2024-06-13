// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTSChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class ASCEND_API UBTSChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTSChangeSpeed();
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "yes"))
	float AIMovementSpeed = 500.0f;
};
