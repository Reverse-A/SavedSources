// @2023 All rights reversed by Reverse-Alpha Studios


#include "AIControllerModule.h"

#include "AIEntityModule.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Ascend/Character/BaseCharacter.h"

static FName NAME_SightConfiguration(TEXT("Sight Configuration"));
static FName NAME_PerceptionComponent(TEXT("Perception Component"));

AAIControllerModule::AAIControllerModule(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AAIControllerModule::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AAIEntityModule* const AIEntity = Cast<AAIEntityModule>(InPawn))
	{
		if (UBehaviorTree* const BehaviourTree = AIEntity->GetBehaviorTree())
		{
			UBlackboardComponent* BlackBoardComponent;

			UseBlackboard(BehaviourTree->BlackboardAsset, BlackBoardComponent);		
			Blackboard = BlackBoardComponent;
			RunBehaviorTree(BehaviourTree);
		}
	}
}

void AAIControllerModule::SetupPerceptionSystem()
{
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(NAME_SightConfiguration);

	if (SightConfiguration)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(NAME_PerceptionComponent));
		SightConfiguration->SightRadius = 500.0f;
		SightConfiguration->LoseSightRadius = SightConfiguration->SightRadius + 25.0f;
		SightConfiguration->PeripheralVisionAngleDegrees = 90.0f;
		SightConfiguration->SetMaxAge(5.0f);
		SightConfiguration->AutoSuccessRangeFromLastSeenLocation		= 520.0f;
		SightConfiguration->DetectionByAffiliation.bDetectEnemies		= true;
		SightConfiguration->DetectionByAffiliation.bDetectFriendlies	= true;
		SightConfiguration->DetectionByAffiliation.bDetectNeutrals		= true;

		GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerModule::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
	}
}

void AAIControllerModule::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const BaseCharacter = Cast<ABaseCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
