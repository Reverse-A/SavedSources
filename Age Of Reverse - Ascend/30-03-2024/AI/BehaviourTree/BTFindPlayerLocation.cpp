// @2023 All rights reversed by Reverse-Alpha Studios


#include "BTFindPlayerLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

UBTFindPlayerLocation::UBTFindPlayerLocation(FObjectInitializer const& ObjectInitializer)
: UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get ABaseCharacter
	if (auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// Get Player Location to use as an origin
		auto const PlayerLocation = Player->GetActorLocation();

		if (SearchRandom)
		{
			FNavLocation NavigationLocation;

			// Get the navigation system and generate a random location near the player
			if (auto* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				// try to get a random location near the player
				if (NavigationSystem->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, NavigationLocation))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavigationLocation.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
