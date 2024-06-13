// @2023 All rights reversed by Reverse-Alpha Studios


#include "BTIncrementPathIndex.h"
#include "Ascend/AI/AIControllerModule.h"
#include "Ascend/AI/AIEntityModule.h"
#include "Ascend/AI/AIPatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTIncrementPathIndex::UBTIncrementPathIndex(FObjectInitializer const& ObjectInitializer)
: UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTIncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AIController
	if (auto* const AIController = Cast<AAIControllerModule>(OwnerComp.GetAIOwner()))
	{
		// Get AIModuleCharacter
		if (auto* const AIEntity = Cast<AAIEntityModule>(AIController->GetPawn()))
		{
			// Get Blackboard
			if (auto* const BlackBoardComponent = OwnerComp.GetBlackboardComponent())
			{
				// Get number of patrol points and min and max indices
				auto const NumberOfPoints = AIEntity->GetAIPatrolPath()->Num();
				auto const MinIndex = 0;
				auto const MaxIndex = NumberOfPoints - 1;
				auto Index = BlackBoardComponent->GetValueAsInt(GetSelectedBlackboardKey());

				// Change direction if we are at the first or last index if we are in bidirectional mode

				if (bBiDirection)
				{
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}

				// write new value of index to blackboard
				BlackBoardComponent->SetValueAsInt
				(
					GetSelectedBlackboardKey(),
					(Direction == EDirectionType::Forward ? ++Index : --Index) % NumberOfPoints
				);
					
				// Finish Task
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
				
			}

		}
	}
	return EBTNodeResult::Failed;
}
