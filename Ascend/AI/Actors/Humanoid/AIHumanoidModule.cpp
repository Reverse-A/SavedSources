// @2023 All rights reversed by Reverse-Alpha Studios


#include "AIHumanoidModule.h"

AAIHumanoidModule::AAIHumanoidModule()
{
	PrimaryActorTick.bStartWithTickEnabled = false;

}

void AAIHumanoidModule::BeginPlay()
{
	Super::BeginPlay();

	AIHumanoidAnimInstance = Cast<UAIHumanoidAnimInstance>(GetMesh()->GetAnimInstance());
	if (AIHumanoidAnimInstance)
	{
		
	}
}
