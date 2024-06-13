// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTIncrementPathIndex.generated.h"

/**
 * 
 */
UCLASS()
class ASCEND_API UBTIncrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTIncrementPathIndex(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	enum class EDirectionType 
	{
		Forward,
		Reverse
	};

	EDirectionType Direction = EDirectionType::Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bBiDirection = false;
};
