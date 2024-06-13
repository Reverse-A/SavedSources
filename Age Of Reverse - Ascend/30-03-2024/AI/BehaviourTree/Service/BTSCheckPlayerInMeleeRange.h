// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTSCheckPlayerInMeleeRange.generated.h"

/**
 * 
 */
UCLASS()
class ASCEND_API UBTSCheckPlayerInMeleeRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTSCheckPlayerInMeleeRange();

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float MeleeRange = 10.0f;
};
