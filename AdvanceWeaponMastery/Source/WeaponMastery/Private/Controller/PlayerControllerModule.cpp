#include "Controller/PlayerControllerModule.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "UObject/ConstructorHelpers.h"

#include <WeaponMastery/Public/Character/CharacterModule.h>
#include <WeaponMastery/Public/Component/ArsenalComponent.h>
#include <WeaponMastery/Public/Utilities/DiagnosticSystem.h>

APlayerControllerModule::APlayerControllerModule()
{
	// Define asset paths for input mapping context and actions
	FString DefaultMappingContextPath	= TEXT("/WeaponMastery/Blueprint/Input/IMC_Default.IMC_Default");
	FString MoveActionPath				= TEXT("/WeaponMastery/Blueprint/Input/Actions/IA_Move.IA_Move");
	FString LookActionPath				= TEXT("/WeaponMastery/Blueprint/Input/Actions/IA_Look.IA_Look");
	FString JumpActionPath				= TEXT("/WeaponMastery/Blueprint/Input/Actions/IA_Jump.IA_Jump");
	FString InteractionActionPath		= TEXT("/WeaponMastery/Blueprint/Input/Actions/IA_Interaction.IA_Interaction");
	FString ThrowActionPath				= TEXT("/WeaponMastery/Blueprint/Input/Actions/IA_Throw.IA_Throw");

	// Directly load the assets using the paths
	DefaultMappingContext	= LoadObject<UInputMappingContext>	(nullptr, *DefaultMappingContextPath);
	MoveAction				= LoadObject<UInputAction>			(nullptr, *MoveActionPath);
	LookAction				= LoadObject<UInputAction>			(nullptr, *LookActionPath);
	JumpAction				= LoadObject<UInputAction>			(nullptr, *JumpActionPath);
	InteractionAction		= LoadObject<UInputAction>			(nullptr, *InteractionActionPath);
	ThrowAction				= LoadObject<UInputAction>			(nullptr, *ThrowActionPath);
}

void APlayerControllerModule::BeginPlay()
{
	// Call the parent class's BeginPlay function
	Super::BeginPlay();

	// Check if the EnhancedInputLocalPlayerSubsystem is available
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// Add the default mapping context to the subsystem
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	else
	{
		// Log an error message if the EnhancedInputLocalPlayerSubsystem is not available
		LOG_INVALID(Subsystem);
	}
}


void APlayerControllerModule::SetupInputComponent()
{
	// Call the parent class's SetupInputComponent function
	Super::SetupInputComponent();

	// Check if the input component is of type UEnchancedInputComponent
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Binding actions to functions
		if (MoveAction != nullptr)
		{
			// Bind the MoveAction to the Move function
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerControllerModule::Move);
		}
		else
		{
			LOG_INVALID(MoveAction);
		}

		if (LookAction != nullptr)
		{
			// Bind the LookAction to the Look function
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerControllerModule::Look);
		}
		else
		{
			LOG_INVALID(LookAction);
		}

		if (JumpAction != nullptr)
		{
			// Bind the JumpAction to the Jump function when triggered
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerControllerModule::Jump);
			// Bind the JumpAction to the StopJump function when completed
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerControllerModule::StopJump);
		}
		else
		{
			LOG_INVALID(JumpAction);
		}

		if (InteractionAction != nullptr)
		{
			// Bind the InteractionAction to the InteractionInput function when started
			EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &APlayerControllerModule::InteractionInput);
		}
		else
		{
			LOG_INVALID(InteractionAction);
		}

		if (ThrowAction != nullptr)
		{
			// Bind the ThrowAction to the ThrowInput function when started
			EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &APlayerControllerModule::ThrowInput);
		}
		else
		{
			LOG_INVALID(ThrowAction);
		}
	}
}

void APlayerControllerModule::Move(const FInputActionValue& Value)
{
	// Get the movement vector from the input action value
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Check if the player controller has a pawn (character) associated with it
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Check if the controlled pawn is a character
		if (ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn))
		{
			// Add movement input based on the movement vector
			ControlledCharacter->AddMovementInput(ControlledCharacter->GetActorForwardVector(), MovementVector.Y);
			ControlledCharacter->AddMovementInput(ControlledCharacter->GetActorRightVector(), MovementVector.X);
		}
		else
		{
			// Log an invalid controlled character if the cast fails
			LOG_INVALID(ControlledCharacter);
		}
	}
	else
	{
		// Log an error message if the controller is null
		LOG_INVALID(ControlledPawn);
	}
}

void APlayerControllerModule::Look(const FInputActionValue& Value)
{
	// Get the look axis vector from the input action value
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Check if the player controller has a pawn (character) associated with it
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Check if the controlled pawn is a character
		if (ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn))
		{
			// Add yaw and pitch input to the controller
			AddYawInput(LookAxisVector.X);
			AddPitchInput(LookAxisVector.Y);
		}
		else
		{
			// Log an invalid controlled character if the cast fails
			LOG_INVALID(ControlledCharacter);
		}
	}
	else
	{
		// Log an error message if the controller is null
		LOG_INVALID(ControlledPawn);
	}
}


void APlayerControllerModule::Jump()
{
	// Check if the player controller has a pawn (character) associated with it
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Check if the controlled pawn is a character
		if (ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn))
		{
			// Make the controlled character jump
			ControlledCharacter->Jump();
		}
		else
		{
			// Log an invalid controlled character if the cast fails
			LOG_INVALID(ControlledCharacter);
		}
	}
	else
	{
		// Log an error message if the controller is null
		LOG_INVALID(ControlledPawn);
	}
}

void APlayerControllerModule::StopJump()
{
	// Check if the player controller has a pawn (character) associated with it
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Check if the controlled pawn is a character
		if (ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn))
		{
			// Make the controlled character stop jumping
			ControlledCharacter->StopJumping();
		}
		else
		{
			// Log an invalid controlled character if the cast fails
			LOG_INVALID(ControlledCharacter);
		}
	}
	else
	{
		// Log an error message if the controller is null
		LOG_INVALID(ControlledPawn);
	}
}

// Function handling interaction input for the player controller
void APlayerControllerModule::InteractionInput()
{
	// Get the controlled pawn of the player
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Cast the controlled pawn to ACharacter to ensure it's a character
		if (ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn))
		{
			// Cast the controlled character to ACharacterModule to access specific character functionality
			if (ACharacterModule* CharacterModule = Cast<ACharacterModule>(ControlledCharacter))
			{
				// Get the arsenal component from the character module
				UArsenalComponent* ArsenalComponent = CharacterModule->GetArsenalComponent();

				// Check if the arsenal component is valid
				if (ArsenalComponent != nullptr)
				{
					// Equip the pending pickup weapon if the arsenal component is valid
					ArsenalComponent->EquipPendingPickupWeapon();
				}
				else
				{
					// Log an invalid arsenal component if it's null
					LOG_INVALID(ArsenalComponent);
				}
			}		
			else
			{
				// Log an invalid character module if the cast fails
				LOG_INVALID(CharacterModule);
			}
		}
		else
		{
			// Log an invalid controlled character if the cast fails
			LOG_INVALID(ControlledCharacter);
		}
	}
	else
	{
		// Log an invalid controlled pawn if it's null
		LOG_INVALID(ControlledPawn);
	}
}

// Function handling throw input for the player controller
void APlayerControllerModule::ThrowInput()
{
	// Get the controlled pawn of the player
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Cast the controlled pawn to ACharacter to ensure it's a character
		if (ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn))
		{
			// Cast the controlled character to ACharacterModule to access specific character functionality
			if (ACharacterModule* CharacterModule = Cast<ACharacterModule>(ControlledCharacter))
			{
				// Get the arsenal component from the character module
				UArsenalComponent* ArsenalComponent = CharacterModule->GetArsenalComponent();

				// Check if the arsenal component is valid
				if (ArsenalComponent != nullptr)
				{
					// Execute the weapon throw if the arsenal component is valid
					ArsenalComponent->ExecuteWeaponThrow();
				}
				else
				{
					// Log an invalid arsenal component if it's null
					LOG_INVALID(ArsenalComponent);
				}
			}
			else
			{
				// Log an invalid character module if the cast fails
				LOG_INVALID(CharacterModule);
			}
		}
		else
		{
			// Log an invalid controlled character if the cast fails
			LOG_INVALID(ControlledCharacter);
		}
	}
	else
	{
		// Log an invalid controlled pawn if it's null
		LOG_INVALID(ControlledPawn);
	}
}