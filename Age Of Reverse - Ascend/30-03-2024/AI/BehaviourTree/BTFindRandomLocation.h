// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTFindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class ASCEND_API UBTFindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTFindRandomLocation(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float SearchRadius = 1500.0f;

};
