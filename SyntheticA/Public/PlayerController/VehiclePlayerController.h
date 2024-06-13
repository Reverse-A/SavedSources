// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VehiclePlayerController.generated.h"

class UInputMappingContext;
class AGroundVehiclePawn;

UCLASS()
class SYNTHETICA_API AVehiclePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* InputMappingContext;

	/** Pointer to the controlled vehicle pawn */
	TObjectPtr<AGroundVehiclePawn> VehiclePawn;
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float Delta) override;
protected:
	virtual void OnPossess(APawn* InPawn) override;
};
