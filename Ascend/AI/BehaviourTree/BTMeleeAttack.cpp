// @2023 All rights reversed by Reverse-Alpha Studios


#include "BTMeleeAttack.h"
#include "Ascend/AI/AIEntityModule.h"
#include "Ascend/AI/AIControllerModule.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

static FString NAME_MeleeAttack(TEXT("Melee Attack"));

UBTMeleeAttack::UBTMeleeAttack()
{
	NodeName = NAME_MeleeAttack;
}

EBTNodeResult::Type UBTMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// if we out of range, do not attack the player
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		// Finish Task
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		return EBTNodeResult::Succeeded;
	}

	// we are in range so get the AI's controller and the AI's entity itself
	auto const* const AIController = OwnerComp.GetAIOwner();
	auto* const AIEntity = Cast<AAIEntityModule>(AIController->GetPawn());

	// if the NPC supports the ICombatInterface, cast and call the Execute_MeleeAttack function
	if (auto* const ICombat = Cast<ICombatInterface>(AIEntity))
	{
		// Necessary check to see if the montage has finished so we don't try and play it again
		if (MontageHasFinished(AIEntity))
		{
			ICombat->Execute_MeleeAttack(AIEntity);
		}
	}

	// Finish Task
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTMeleeAttack::MontageHasFinished(AAIEntityModule* const AIEntity)
{
	return AIEntity->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(AIEntity->GetAttackMontage());
}
