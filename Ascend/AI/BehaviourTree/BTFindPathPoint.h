// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTFindPathPoint.generated.h"

/**
 * 
 */
UCLASS()
class ASCEND_API UBTFindPathPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UBTFindPathPoint(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta= (AllowPrivateAccess = "true"))
	FBlackboardKeySelector PatrolPathVectorKey;
};
