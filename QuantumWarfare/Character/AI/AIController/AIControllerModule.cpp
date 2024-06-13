// @2023 All rights reversed by Reverse-Alpha Studios


#include "AIControllerModule.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AAIControllerModule::AAIControllerModule()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard)

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviourTreeComponent");
	check(BehaviorTreeComponent)
}