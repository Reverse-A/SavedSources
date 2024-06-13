// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerModule.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class QUANTUMWARFARE_API AAIControllerModule : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIControllerModule();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

};
