// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerController/VehiclePlayerController.h"
#include "Vehicle/GroundVehiclePawn.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void AVehiclePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AVehiclePlayerController::Tick(float Delta)
{

}

void AVehiclePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	VehiclePawn = CastChecked<AGroundVehiclePawn>(InPawn);
}