// @2023 All rights reversed by Reverse-Alpha Studios


#include "BTChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Ascend/AI/AIControllerModule.h"

UBTChasePlayer::UBTChasePlayer(FObjectInitializer const& ObjectInitializer)
: UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get target location from blackboard via the NPC's controller

	if (auto* const AIController = Cast<AAIControllerModule>(OwnerComp.GetAIOwner()))
	{
		auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

		// move to the player's location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, PlayerLocation);

		// Finish Task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
