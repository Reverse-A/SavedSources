/*
* Copyright (c) Reverse-A @2024
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerModule.generated.h"

 // Forward declarations of classes
class UInputAction;
class UInputMappingContext;

// Forward declaration of a struct
struct FInputActionValue;

UCLASS(BlueprintType)
class WEAPONMASTERY_API APlayerControllerModule : public APlayerController
{
	GENERATED_BODY()

public:
    // Constructor for the class
    APlayerControllerModule();

protected:
    // Function that is called when the game starts or when the actor is spawned
    virtual void BeginPlay() override;

    // Function to set up the input component. This is where input bindings are defined.
    virtual void SetupInputComponent() override;

protected:
    // Function to handle movement input
    void Move(const FInputActionValue& Value);

    // Function to handle looking around (camera movement) input
    void Look(const FInputActionValue& Value);

    // Function to handle jump input
    void Jump();

    // Function to handle stop jumping input
    void StopJump();

    // Function to handle interaction input (e.g., picking up weapons from the ground)
    void InteractionInput();

    // Function to handle input for throwing an equipped weapon and detaching it from the skeletal mesh's socket
    void ThrowInput();
private:
    /** Default Input Mapping Context */
    // This property holds a reference to the default input mapping context, which defines how inputs are mapped to actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    /** Move Input Action */
    // This property holds a reference to the move input action, which is triggered when the player moves
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> MoveAction;

    /** Look Input Action */
    // This property holds a reference to the look input action, which is triggered when the player looks around
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> LookAction;

    /** Jump Input Action */
    // This property holds a reference to the jump input action, which is triggered when the player jumps
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> JumpAction;

    /** Interaction Input Action */
    // This property holds a reference to the interaction input action, which is used for interacting with objects (e.g., picking up weapons)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> InteractionAction;

    /** Throw Input Action */
    // This property holds a reference to the throw input action, which is used to throw the equipped weapon
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> ThrowAction;
};
