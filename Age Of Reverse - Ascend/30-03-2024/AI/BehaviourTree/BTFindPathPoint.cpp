// @2023 All rights reversed by Reverse-Alpha Studios


#include "BTFindPathPoint.h"

#include "AIModule.h"
#include "Ascend/AI/AIPatrolPath.h"
#include "Ascend/AI/AIControllerModule.h"
#include "Ascend/AI/AIEntityModule.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTFindPathPoint::UBTFindPathPoint(FObjectInitializer const& ObjectInitializer)
: UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTFindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Attempt to get the AIControllerModule<
	if (auto* const AIController = Cast<AAIControllerModule>(OwnerComp.GetAIOwner()))
	{
		// Attempt to get the blackboard component from the behaviour tree
		if (auto* const BlackboardComponent = OwnerComp.GetBlackboardComponent())
		{
			// Get the current patrol path index from the blackboard
			auto const Index = BlackboardComponent->GetValueAsInt((GetSelectedBlackboardKey()));

			// Get the AIEntity
			if (auto* AIEntity = Cast<AAIEntityModule>(AIController->GetPawn()))
			{
				// Get the current patrol path vector from the NPC - this is local to the patrol path actor
				auto const Point = AIEntity->GetAIPatrolPath()->GetAIPatrolPoint(Index);

				// Convert the local vector to a global point
				auto const GlobalPoint = AIEntity->GetAIPatrolPath()->GetActorTransform().TransformPosition(Point);
				BlackboardComponent->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				// Finish Task
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
