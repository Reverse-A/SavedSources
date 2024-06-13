// @2023 All rights reversed by Reverse-Alpha Studios


#include "BTFindRandomLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Ascend/AI/AIControllerModule.h"

UBTFindRandomLocation::UBTFindRandomLocation(FObjectInitializer const& ObjectInitializer)
: UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = "Find Random Location In NavMesh";

}

EBTNodeResult::Type UBTFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AI controller 
	if (AAIControllerModule* const AIController = Cast<AAIControllerModule>(OwnerComp.GetAIOwner()))
	{
		if (auto* const AIEntity = AIController->GetPawn())
		{
			// Obtain AIPawn location to use as an origin
			auto const Origin = AIEntity->GetActorLocation();

			// Get the navigation system and generate a random location
			if (auto* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation NavigationLocation;
				
				if (NavigationSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, NavigationLocation))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavigationLocation.Location);
				}

				// Finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
