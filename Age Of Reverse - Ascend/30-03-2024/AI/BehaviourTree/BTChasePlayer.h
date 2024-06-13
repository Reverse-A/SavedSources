// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class ASCEND_API UBTChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UBTChasePlayer(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
