// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerModule.generated.h"

class UAISenseConfig_Sight;
UCLASS()
class ASCEND_API AAIControllerModule : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit AAIControllerModule(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UAISenseConfig_Sight* SightConfiguration;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
