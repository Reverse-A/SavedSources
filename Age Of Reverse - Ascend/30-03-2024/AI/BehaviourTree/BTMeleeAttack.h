// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTMeleeAttack.generated.h"

class AAIEntityModule;

UCLASS()
class ASCEND_API UBTMeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTMeleeAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool MontageHasFinished(AAIEntityModule* const AIEntity);
};
