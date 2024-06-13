// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerControllerModule.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

#include <QuantumWarfare/Ability/AbilitySystem.h>
#include <QuantumWarfare/Character/PlayerCharacter.h>
#include <QuantumWarfare/Input/PlayerInputComponent.h>
#include <QuantumWarfare/Interaction/EnemyInterface.h>
#include <QuantumWarfare/UI/Widget/DamageTextComponent.h>
#include "QuantumWarfare/QWGameplayTags.h"

FName NAME_PCSplineComp("Spline");

APlayerControllerModule::APlayerControllerModule()
{
	bReplicates = true;

    Spline = CreateDefaultSubobject<USplineComponent>(NAME_PCSplineComp);
}

void APlayerControllerModule::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    CursorTrace();
    AutoRun();
}

void APlayerControllerModule::BeginPlay()
{
    // Calls the BeginPlay function of the parent class (Superclass)
     Super::BeginPlay();

     PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
     if (PlayerCharacter != nullptr)
     {
         CharacterMovementComponent = PlayerCharacter->GetCharacterMovement();
     }

     // Checks if PlayerContext is valid, if not, it will trigger an assertion (error) and stop execution
    check(PlayerContext);

    // Get a reference to the Enhanced Input Local Player Subsystem associated with the local player
    class UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    // Checks if Subsystem is valid, if not, it will trigger an assertion (error) and stop execution
    if (Subsystem)
    {
        // Add the PlayerContext mapping context to the Enhanced Input Local Player Subsystem
        Subsystem->AddMappingContext(PlayerContext, 0);
    }

    // Sets whether the mouse cursor is visible
    bShowMouseCursor = true;

    // Sets the default mouse cursor to use
    DefaultMouseCursor = EMouseCursor::Default;

    // Configures the input mode for the player controller to allow both game and UI input
    FInputModeGameAndUI InputModeData;
    // Sets the behavior for locking the mouse cursor to the viewport
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    // Sets whether to hide the cursor during input capture
    InputModeData.SetHideCursorDuringCapture(false);
    // Sets the input mode for the player controller
    SetInputMode(InputModeData);
}

void APlayerControllerModule::SetupInputComponent()
{
    //Calls the SetupInputComponent function of the parent class (Superclass)
    Super::SetupInputComponent();
    //Casts the InputComponent to UEnhancedInputComponent and ensures it's valid
    UPlayerInputComponent* PlayerInputComponent = CastChecked<UPlayerInputComponent>(InputComponent);

    /*Movement*/
    PlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerControllerModule::Move);

    /*Sprint*/
    PlayerInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerControllerModule::Sprint);
    PlayerInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerControllerModule::Sprint_Completed);
    PlayerInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &APlayerControllerModule::Interaction);
    PlayerInputComponent->BindAction(HoldPlayerAction, ETriggerEvent::Started, this, &APlayerControllerModule::HoldPlayerKeyPressed);
    PlayerInputComponent->BindAction(HoldPlayerAction, ETriggerEvent::Completed, this, &APlayerControllerModule::HoldPlayerKeyReleased);

    PlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerControllerModule::Jump);

    PlayerInputComponent->BindAbilityActions
    (
        InputConfig,
        this, 
        &ThisClass::AbilityInputTagPressed,
        &ThisClass::AbilityInputTagReleased, 
        &ThisClass::AbilityInputTagHeld
    );

}
#pragma region InputFunctions
void APlayerControllerModule::Move(const FInputActionValue& InputActionValue)
{
    // 1. Gets the input axis vector from the InputActionValue
    // 2. Gets the control rotation and constructs a rotation for yaw only
    // 3. Gets the forward and right direction vectors based on the yaw rotation
    // 4. Gets the controlled pawn associated with this player controller
    // 5. Adds movement input to the controlled pawn based on the input axis vector

/*1*/
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

/*2*/
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

/*3*/
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

/*4*/
    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
/*5*/
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}

void APlayerControllerModule::AutoRun()
{
    if (!bAutoRunning) return;
    if (APawn* ControlledPawn = GetPawn())
    {
        const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation
        (
            ControlledPawn->GetActorLocation(),
            ESplineCoordinateSpace::World
        );

        const FVector Direction = Spline->FindDirectionClosestToWorldLocation
        (
            LocationOnSpline,
            ESplineCoordinateSpace::World
        );
        
        ControlledPawn->AddMovementInput(Direction);

        const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();

        if (DistanceToDestination <= AutoRunAcceptanceRadius)
        {
            bAutoRunning = false;
        }
    }
}

void APlayerControllerModule::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockHit, bool bCriticalHit)
{
    if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
    {
        UDamageTextComponent* DamageText =  NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
        DamageText->RegisterComponent();
        DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        DamageText->SetDamageText(DamageAmount, bBlockHit, bCriticalHit);
    }
}

void APlayerControllerModule::Sprint()
{
    if (!HasAuthority())
    {
        CharacterMovementComponent->MaxWalkSpeed = 550.0f;
    }
    else
    {
        // Get the current velocity
        float CurrentVelocity = PlayerCharacter->GetVelocity().Size();
        float MaxSprintVelocity = 550.f;

        // Calculate the target velocity increment per second
        float TargetVelocityIncrement = (MaxSprintVelocity - CurrentVelocity) / 2.5f;

        // Interpolate the current velocity towards the max sprint velocity over 2.5 seconds
        float NewVelocity = FMath::FInterpTo
        (
            CurrentVelocity,
            MaxSprintVelocity,
            GetWorld()->GetDeltaSeconds(),
            2.5f
        );

        CharacterMovementComponent->MaxWalkSpeed = NewVelocity;
    }
}

void APlayerControllerModule::Sprint_Completed()
{
    float DefaultMaxWalkSpeed = 350.0f;
    CharacterMovementComponent->MaxWalkSpeed = DefaultMaxWalkSpeed;;
}

void APlayerControllerModule::Interaction()
{
    if (PlayerCharacter == nullptr)
    {
        PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    }

    PlayerCharacter->Interaction();
}

void APlayerControllerModule::Jump()
{
    PlayerCharacter->Jump();
}
#pragma endregion

#pragma region AbilityInputTagFunctions
void APlayerControllerModule::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (InputTag.MatchesTagExact(FQWGameplayTags::Get().InputTag_LMB))
    {
        bTargeting      = ThisActor ? true : false;
        bAutoRunning    = false;
    }
}

void APlayerControllerModule::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (!InputTag.MatchesTagExact(FQWGameplayTags::Get().InputTag_LMB))
    {
        if (GetAbilitySystem())
        {
            GetAbilitySystem()->AbilityInputTagReleased(InputTag);
        }

        return;
    }

    if (GetAbilitySystem())
    {
        GetAbilitySystem()->AbilityInputTagReleased(InputTag);
    }

    if (!bTargeting && !bHoldPlayerKeyDown)
    {
        const APawn* ControlledPawn = GetPawn();
        if (FollowTime <= ShortPressThreshold && ControlledPawn)
        {
            if (UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
            {
                Spline->ClearSplinePoints();

                for (const FVector& PointLocation : NavigationPath->PathPoints)
                {
                    Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
                }
                CachedDestination = NavigationPath->PathPoints[NavigationPath->PathPoints.Num() - 1];
                bAutoRunning = true;
            }
        }
        FollowTime = 0.0f;
        bTargeting = false;
    }
}

void APlayerControllerModule::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (!InputTag.MatchesTagExact(FQWGameplayTags::Get().InputTag_LMB))
    {
        if (GetAbilitySystem())
        {
            GetAbilitySystem()->AbilityInputTagHeld(InputTag);
        }
        return;
    }

    if (bTargeting || bHoldPlayerKeyDown)
    {
        if (GetAbilitySystem())
        {
            GetAbilitySystem()->AbilityInputTagHeld(InputTag);
        }
    }
    else
    {
        FollowTime += GetWorld()->GetDeltaSeconds();

        if (CursorHit.bBlockingHit)
        {
            CachedDestination = CursorHit.ImpactPoint;
        }

        if (APawn* ControlledPawn = GetPawn())
        {
            const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
            ControlledPawn->AddMovementInput(WorldDirection);
        }
    }
}

#pragma endregion

#pragma region CursorTracers
void APlayerControllerModule::CursorTrace()
{

    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

    if (!CursorHit.bBlockingHit) return;

    LastActor = ThisActor;
    ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

    if (LastActor != ThisActor)
    {
        if (LastActor) LastActor->UnHighlightActor();
        if (ThisActor) ThisActor->HighlightActor();
    }

    /*
    * Line trace from cursor. There are several scenarios:
    * A: LastActor is null && ThisActor is null
    *       - Do nothing.
    *
    * B: LastActor is null && ThisActor is valid
    *       - Highlight ThisActor
    *
    * C: LastActor is valid && ThisActor is null
    *       - UnHighLight LastActor
    *
    * D: Both actors are valid, but LastActor != ThisActor
    *       - UnHighLight LastActor, and Highlight This Actor
    *
    * E: Both actors are valid, and are the same actor
    *       - Do nothing
    */

    if (LastActor == nullptr)
    {
        if (ThisActor != nullptr)
        {
            // Case B
            ThisActor->HighlightActor();
        }
        else
        {
            // Case A, both are null, do nothing.!
        }
    }
    else // LastActor is valid
    {
        if (ThisActor == nullptr)
        {
            // Case C
            LastActor->UnHighlightActor();
        }
        else // Both actors are valid 
        {
            if (LastActor != ThisActor)
            {
                // Case D
                LastActor->UnHighlightActor();
                ThisActor->HighlightActor();
            }
            else
            {
                // Case E - do nothing.
            }
        }
    }
}
#pragma endregion


UAbilitySystem* APlayerControllerModule::GetAbilitySystem()
{
    if (AbilitySystem == nullptr)
    {
       AbilitySystem = Cast<UAbilitySystem>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
    }
    return AbilitySystem;
}

