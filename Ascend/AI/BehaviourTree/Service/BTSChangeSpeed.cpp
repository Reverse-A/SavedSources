// @2023 All rights reversed by Reverse-Alpha Studios


#include "BTSChangeSpeed.h"

#include "Ascend/AI/AIEntityModule.h"
#include "Ascend/AI/AIControllerModule.h"

#include "GameFramework/CharacterMovementComponent.h"


UBTSChangeSpeed::UBTSChangeSpeed()
{
	bNotifyBecomeRelevant = true;

	NodeName = TEXT("Change Movement Speed");
}

void UBTSChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (auto* const AIController = OwnerComp.GetAIOwner())
	{
		if (auto* const AIEntity = Cast<AAIEntityModule>(AIController->GetPawn()))
		{
			AIEntity->GetCharacterMovement()->MaxWalkSpeed = AIMovementSpeed;
		}
	}
}
