// @2023 All rights reversed by Reverse-Alpha Studios


#include "BTSCheckPlayerInMeleeRange.h"
#include "Ascend/AI/AIControllerModule.h"
#include "Ascend/AI/AIEntityModule.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTSCheckPlayerInMeleeRange::UBTSCheckPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTSCheckPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AIController and AIEntity
	auto const* const AIController = Cast<AAIControllerModule>(OwnerComp.GetAIOwner());
	auto const* const AIEntity = Cast<AAIEntityModule>(AIController->GetPawn());

	// Get Player Character
	auto const* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// write true or false to the blackboard key depending on whether or not the player is in range.

	OwnerComp.GetBlackboardComponent()->SetValueAsBool
	(
		GetSelectedBlackboardKey(),
		AIEntity->GetDistanceTo(Player) <= MeleeRange
	);
}