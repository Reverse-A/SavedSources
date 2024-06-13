#include "Actor/WeaponModule.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include <WeaponMastery/Public/Character/CharacterModule.h>

static FName NAME_WeaponMesh(TEXT("WeaponMesh"));
static FName NAME_AreaCollision(TEXT("Area Collision"));
static FName NAME_AdvanceWeaponMastery(TEXT("Advance Weapon Mastery"));

AWeaponModule::AWeaponModule()
{
    // Disable ticking for this actor as it does not need to update every frame.
    PrimaryActorTick.bCanEverTick = false;

    // Enable replication to support multiplayer functionality.
    bReplicates = true;

    // Create and initialize the WeaponMesh component.
    // UStaticMeshComponent represents a static mesh that can be rendered in the scene.
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(NAME_WeaponMesh);
    // Ensure that the WeaponMesh component was successfully created.
    check(WeaponMesh)

    // Create and initialize the AreaCollision component.
    // USphereComponent represents a sphere-shaped collision component.
    AreaCollision = CreateDefaultSubobject<USphereComponent>(NAME_AreaCollision);
    // Ensure that the AreaCollision component was successfully created.
    check(AreaCollision);

    // Set the WeaponMesh as the root component of the actor.
    // The root component serves as the base for attaching other components.
    SetRootComponent(WeaponMesh);

    // Attach the AreaCollision component to the WeaponMesh.
    // This ensures the collision component moves with the weapon mesh.
    AreaCollision->SetupAttachment(WeaponMesh);

    // Set the radius of the sphere collision component.
    // The radius determines the size of the collision sphere.
    AreaCollision->SetSphereRadius(100.0f);

    // Set the collision type for the WeaponMesh component to WorldDynamic.
    // WorldDynamic is used for objects that can move and interact within the world.
    WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

    // Configure the AreaCollision component to overlap with objects on the Visibility collision channel.
    // This means the collision will trigger overlap events with objects on this channel.
    AreaCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);


    // Create and initialize the WeaponMasteryComponent.
    // UWeaponMasteryComponent handles weapon mastery data and effects e.t.c
    WeaponMasteryComponent = CreateDefaultSubobject<UWeaponMasteryComponent>(NAME_AdvanceWeaponMastery);

    if (WeaponMasteryComponent != nullptr)
    {
        WeaponMasteryComponent->SetWeaponModule(this);
    }
}
void AWeaponModule::BeginPlay()
{
	Super::BeginPlay();
	HandleWeaponStates();
}


#pragma region CollisionOverlapEvent
// Currently, this system is not in active use. 
// It has been designed as a framework that provides flexibility for incorporating additional functionalities as required in the future.
// Instead of relying on this system, we utilize the LineTrace method to obtain objects.
// LineTrace offers a versatile approach for detecting and interacting with objects within the game environment,
// providing greater control and efficiency in handling interactions.


// This function is called when another object overlaps with this weapon's collision component.
void AWeaponModule::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the overlapping actor is valid (not null).
    if (OtherActor != nullptr)
    {
        // Cast the overlapping actor to ACharacterModule and store it in OwningCharacter.  
        // This ensures that we only respond to overlaps with characters of type ACharacterModule.   
         OwningCharacter = Cast<ACharacterModule>(OtherActor);

        // If the cast was successful and OwningCharacter is valid.
        if (OwningCharacter)
        {
    
        }
    }
}

// This function is called when another object stops overlapping with this weapon's collision component.
void AWeaponModule::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Check if the actor ending the overlap is the owning character.
    if (OtherActor == OwningCharacter)
    {
        // Set OwningCharacter to nullptr to indicate no current character is overlapping.
        OwningCharacter = nullptr;
    }
}


// This function is called when the weapon collides with another object.
void AWeaponModule::OnWeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // TODO: Add logic here to handle various collision scenarios.
    // For example, if the gun falls and collides with the ground,
    // you could play a sound effect indicating impact and trigger
    // a visual effect to simulate the gun bouncing off the surface.

    // Alternatively, if bullets hit the gun, you could implement
    // damage calculation to reduce the gun's health or durability,
    // and possibly trigger an animation to simulate bullet impacts.

    // Check if the collided actor is valid (not null).
    if (OtherActor != nullptr)
    {
        // Check if the collided actor is a static mesh component.
        if (OtherActor->IsA<UStaticMeshComponent>())
        {
            
        }
    }
}

#pragma endregion
#pragma region WeaponState
// Function to set the state of the weapon to a new state.
void AWeaponModule::SetWeaponState(EWeaponState NewState)
{
    // Check if the WeaponMasteryComponent is valid (not null).
    if (WeaponMasteryComponent != nullptr)
    {
        // Retrieve the current weapon mastery data from the WeaponMasteryComponent.
        FWeaponMastery WeaponMastery = WeaponMasteryComponent->GetWeaponMastery();

        EWeaponState ChangedWeaponState = NewState;

        // Update the weapon state in the weapon mastery data to the new state.
        WeaponMastery.WeaponStateType = ChangedWeaponState;
         
        SwitchWeaponState(ChangedWeaponState);
    }
}

// Function to switch the state of the weapon based on the provided weapon state.
void AWeaponModule::SwitchWeaponState(EWeaponState InWeaponState)
{
    // Switch statement to handle different weapon states.
    switch (InWeaponState)
    {
    case EWeaponState::Initial:
        // Call function to handle the initial state of the weapon.
        HandLeInitialState();
        break;

    case EWeaponState::Equipped:
        // Call function to handle the equipped state of the weapon as the primary weapon.
        HandleEquipState();
        break;

    case EWeaponState::EquippedSecondary:
        // Call function to handle the equipped state of the weapon as a secondary weapon.
        HandleEquipSecondaryState();
        break;

    case EWeaponState::Dropped:
        // Call function to handle the dropped state of the weapon.
        HandleDropState();
        break;

    case EWeaponState::Tradeable:
        // Call function to handle the tradeable state of the weapon.
        HandleTradeState();
        break;

    case EWeaponState::Purchasable:
        // Call function to handle the purchasable state of the weapon.
        HandlePurchaseState();
        break;

    case EWeaponState::Lootable:
        // Call function to handle the loot-able state of the weapon.
        HandleLootState();
        break;

    case EWeaponState::Craftable:
        // Call function to handle the craft-able state of the weapon.
        HandleCraftState();
        break;

    case EWeaponState::Upgradable:
        // Call function to handle the upgradable state of the weapon.
        HandleUpgradeState();
        break;

    case EWeaponState::Reward:
        // Call function to handle the reward state of the weapon.
        HandleRewardState();
        break;

    case EWeaponState::Broken:
        // Call function to handle the broken state of the weapon.
        HandleBrokenState();
        break;

    case EWeaponState::InInventory:
        // Call function to handle the state when the weapon is in the player's inventory.
        HandleInventoryState();
        break;

    case EWeaponState::InStorage:
        // Call function to handle the state when the weapon is stored in a safe place.
        HandleStorageState();
        break;

    default:
        break;
    }
}

// Function to handle the weapon states based on the current state retrieved from WeaponMasteryComponent.
void AWeaponModule::HandleWeaponStates()
{
    // Check if the WeaponMasteryComponent is valid (not null).
    if (WeaponMasteryComponent != nullptr)
    {
        // Retrieve the current weapon mastery data from the WeaponMasteryComponent.
        FWeaponMastery WeaponMastery = WeaponMasteryComponent->GetWeaponMastery();

        // Get the current weapon state from the weapon mastery data.
        EWeaponState WeaponState = WeaponMastery.WeaponStateType;

        // Call the function to switch the weapon state based on the retrieved state.
        SwitchWeaponState(WeaponState);
    }
    else
    {
        // Log an error if the WeaponMasteryComponent failed to load.
        UE_LOG(LogTemp, Error, TEXT("Failed to load WeaponMasteryComponent!"));
    }
}

// Function to handle the initial state of the weapon.
void AWeaponModule::HandLeInitialState()
{
    // Bind overlap and end overlap events to respective functions for AreaCollision component.
    AreaCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponModule::OnCollisionOverlap);
    AreaCollision->OnComponentEndOverlap.AddDynamic(this, &AWeaponModule::OnCollisionEndOverlap);

    // Enable overlap events for WeaponMesh component.
    WeaponMesh->SetGenerateOverlapEvents(true);
    // Allow characters to step up on the weapon.
    WeaponMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;

    // Enable query and physics collision for WeaponMesh component.
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    // Set the collision object type for WeaponMesh to WorldDynamic.
    WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

    // Enable physics simulation for WeaponMesh.
    WeaponMesh->SetSimulatePhysics(true);

    // Enable gravity for WeaponMesh.
    WeaponMesh->SetEnableGravity(true);

    // Set collision response for WeaponMesh to block all channels.
    WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    // Ignore collision response for WeaponMesh with pawn channel.
    WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

    // Bind hit event to the function for WeaponMesh component.
    WeaponMesh->OnComponentHit.AddDynamic(this, &AWeaponModule::OnWeaponHit);

    // Disable collision for AreaCollision component.
    AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Ignore collision response for AreaCollision with all channels.
    AreaCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

// Function to handle the state when the weapon is equipped.
void AWeaponModule::HandleEquipState()
{
    // Disable collision for AreaCollision component.
    AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Disable overlap events and physics simulation for WeaponMesh component.
    WeaponMesh->SetGenerateOverlapEvents(false);
    WeaponMesh->SetSimulatePhysics(false);
    WeaponMesh->SetEnableGravity(false);
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Remove bound event handlers for AreaCollision and WeaponMesh components.
    if (AreaCollision->OnComponentBeginOverlap.IsBound())
    {
        AreaCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionOverlap);
    }

    if (AreaCollision->OnComponentEndOverlap.IsBound())
    {
        AreaCollision->OnComponentEndOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionEndOverlap);
    }

    if (WeaponMesh->OnComponentHit.IsBound())
    {
        WeaponMesh->OnComponentHit.RemoveDynamic(this, &AWeaponModule::OnWeaponHit);
    }

    // Check if WeaponMasteryComponent is valid.
    if (WeaponMasteryComponent != nullptr)
    {
        // Retrieve weapon effects from WeaponMasteryComponent.
        FWeaponEffectAndProps WeaponEffectAndProps = WeaponMasteryComponent->GetWeaponEffectAndProps();

        // Get the equip sound cue from weapon effects.
        USoundCue* EquipSound = WeaponEffectAndProps.EquipSoundCue;

        // Play the drop sound cue if it's valid.
        if (EquipSound != nullptr)
        {
            Play2DSound(EquipSound);
        }
        else
        {
            // Display a debug message if the drop sound cue is not set.
            LOG_INVALID(EquipSound);
        }
    }
    else
    {
        // Display a debug message if WeaponMasteryComponent is not valid.
        LOG_INVALID(WeaponMasteryComponent);
    }
}

// Function to handle the state when the weapon is equipped as a secondary weapon.
void AWeaponModule::HandleEquipSecondaryState()
{
    // TODO: Reserved for future use.
    // This function is currently a placeholder for handling the weapon's behavior
    // when it is equipped as a secondary weapon. Future implementations may include:
    // - Disabling primary weapon functions while secondary weapon is active
    // - Adjusting UI elements to indicate secondary weapon status
    // - Changing weapon attributes such as damage or fire rate for secondary use
    // - Managing animations specific to secondary weapon handling
    // - Implementing specific sound cues or visual effects for secondary weapon

    // Log state change for debugging purposes

}

// Function to handle the state when the weapon is dropped.
void AWeaponModule::HandleDropState()
{
    // Configure WeaponMesh for dropped state:
    // Enable overlap events, set collision type, enable physics, enable gravity, and set collision responses.
    WeaponMesh->SetGenerateOverlapEvents(true);  // Allow WeaponMesh to generate overlap events.
    WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);  // Set the collision object type to WorldDynamic.
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // Enable collision for both query and physics interactions.
    WeaponMesh->SetSimulatePhysics(true);  // Enable physics simulation on WeaponMesh.
    WeaponMesh->SetEnableGravity(true);  // Enable gravity for WeaponMesh.
    WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);  // Block all collision channels by default.
    WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);  // Ignore collision with Pawns.

    // Bind the OnWeaponHit function to the WeaponMesh's OnComponentHit event.
    WeaponMesh->OnComponentHit.AddDynamic(this, &AWeaponModule::OnWeaponHit);

    // Configure AreaCollision for dropped state:
    // Enable query-only collision, and set collision responses.
    AreaCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);                                                   // Enable collision for query interactions only.
    AreaCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);         // Overlap with Pawns.
    AreaCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponModule::OnCollisionOverlap);                        // Bind the OnCollisionOverlap function to the AreaCollision's OnComponentBeginOverlap event.
    AreaCollision->OnComponentEndOverlap.AddDynamic(this, &AWeaponModule::OnCollisionEndOverlap);                       // Bind the OnCollisionEndOverlap function to the AreaCollision's OnComponentEndOverlap event.
    AreaCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);     // Block visibility channel.

    // Check if WeaponMasteryComponent is valid.
    if (WeaponMasteryComponent != nullptr)
    {
        // Get the drop sound cue from weapon effects.
        USoundCue* DropSound = WeaponMasteryComponent->GetWeaponEffectAndProps().DropSoundCue;

        // Play the drop sound cue if it's valid.
        if (DropSound != nullptr)
        {
            Play2DSound(DropSound);
        }
        else
        {
            // Display a debug message if the drop sound cue is not set.
            LOG_INVALID(DropSound);
        }
    }
    else
    {
        // Display a debug message if WeaponMasteryComponent is not valid.
        LOG_INVALID(WeaponMasteryComponent);

    }
}

// Function to handle the state when the weapon is in the trade state.
void AWeaponModule::HandleTradeState()
{
    // Configure WeaponMesh for trade state:
    // Disable collision, physics simulation, and gravity.
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for WeaponMesh.
    WeaponMesh->SetSimulatePhysics(false);  // Disable physics simulation for WeaponMesh.
    WeaponMesh->SetEnableGravity(false);  // Disable gravity for WeaponMesh.

    // Configure AreaCollision for trade state:
    // Disable collision.
    AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for AreaCollision.

    // Unbind the OnComponentBeginOverlap event if it is bound.
    if (AreaCollision->OnComponentBeginOverlap.IsBound())
    {
        AreaCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionOverlap);  // Remove the OnCollisionOverlap event binding.
    }

    // Unbind the OnComponentEndOverlap event if it is bound.
    if (AreaCollision->OnComponentEndOverlap.IsBound())
    {
        AreaCollision->OnComponentEndOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionEndOverlap);  // Remove the OnCollisionEndOverlap event binding.
    }

    // Unbind the OnComponentHit event if it is bound.
    if (WeaponMesh->OnComponentHit.IsBound())
    {
        WeaponMesh->OnComponentHit.RemoveDynamic(this, &AWeaponModule::OnWeaponHit);  // Remove the OnWeaponHit event binding.
    }
}

// Function to handle the state when the weapon is available for purchase.
void AWeaponModule::HandlePurchaseState()
{
    // The HandlePurchaseState function is currently a placeholder.
    // In the future, you might want to add logic to handle the purchase state, such as:
    // - Displaying the weapon in a shop interface.
    // - Setting specific visual or audio effects.
    // - Updating the weapon's attributes or state when it is bought.

    // Example (not implemented):
    // ShowWeaponInShop();
    // SetPurchaseEffects();
}

// Function to handle the state when the weapon is available for looting.
void AWeaponModule::HandleLootState()
{
    // The HandleLootState function is currently a placeholder.
    // In the future, you might want to add logic to handle the loot state, such as:
    // - Displaying the weapon in loot containers or on the ground.
    // - Setting specific visual or audio effects.
    // - Updating the weapon's attributes or state when it is looted.

    // Example (not implemented):
    // ShowWeaponInLootContainer();
    // SetLootEffects();
}

// Function to handle the state when the weapon is in the craft state.
void AWeaponModule::HandleCraftState()
{
    // Configure WeaponMesh for craft state:
    // Disable collision, physics simulation, and gravity.
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for WeaponMesh.
    WeaponMesh->SetSimulatePhysics(false);  // Disable physics simulation for WeaponMesh.
    WeaponMesh->SetEnableGravity(false);  // Disable gravity for WeaponMesh.

    // Configure AreaCollision for craft state:
    // Disable collision.
    AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for AreaCollision.

    // Unbind the OnComponentBeginOverlap event if it is bound.
    if (AreaCollision->OnComponentBeginOverlap.IsBound())
    {
        AreaCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionOverlap);  // Remove the OnCollisionOverlap event binding.
    }

    // Unbind the OnComponentEndOverlap event if it is bound.
    if (AreaCollision->OnComponentEndOverlap.IsBound())
    {
        AreaCollision->OnComponentEndOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionEndOverlap);  // Remove the OnCollisionEndOverlap event binding.
    }

    // Unbind the OnComponentHit event if it is bound.
    if (WeaponMesh->OnComponentHit.IsBound())
    {
        WeaponMesh->OnComponentHit.RemoveDynamic(this, &AWeaponModule::OnWeaponHit);  // Remove the OnWeaponHit event binding.
    }
}

// Function to handle the state when the weapon is in the upgrade state.
void AWeaponModule::HandleUpgradeState()
{
    // Configure WeaponMesh for upgrade state:
    // Disable collision, physics simulation, and gravity.
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for WeaponMesh.
    WeaponMesh->SetSimulatePhysics(false);  // Disable physics simulation for WeaponMesh.
    WeaponMesh->SetEnableGravity(false);  // Disable gravity for WeaponMesh.

    // Configure AreaCollision for upgrade state:
    // Disable collision.
    AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for AreaCollision.

    // Unbind the OnComponentBeginOverlap event if it is bound.
    if (AreaCollision->OnComponentBeginOverlap.IsBound())
    {
        AreaCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionOverlap);  // Remove the OnCollisionOverlap event binding.
    }

    // Unbind the OnComponentEndOverlap event if it is bound.
    if (AreaCollision->OnComponentEndOverlap.IsBound())
    {
        AreaCollision->OnComponentEndOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionEndOverlap);  // Remove the OnCollisionEndOverlap event binding.
    }

    // Unbind the OnComponentHit event if it is bound.
    if (WeaponMesh->OnComponentHit.IsBound())
    {
        WeaponMesh->OnComponentHit.RemoveDynamic(this, &AWeaponModule::OnWeaponHit);  // Remove the OnWeaponHit event binding.
    }
}

// Function to handle the state when the weapon is broken.
void AWeaponModule::HandleBrokenState()
{
    // Configure WeaponMesh for broken state:
    // Disable collision, physics simulation, and gravity.
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for WeaponMesh.
    WeaponMesh->SetSimulatePhysics(false);  // Disable physics simulation for WeaponMesh.
    WeaponMesh->SetEnableGravity(false);  // Disable gravity for WeaponMesh.

    // Configure AreaCollision for broken state:
    // Disable collision.
    AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for AreaCollision.

    // Unbind the OnComponentBeginOverlap event if it is bound.
    if (AreaCollision->OnComponentBeginOverlap.IsBound())
    {
        AreaCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionOverlap);  // Remove the OnCollisionOverlap event binding.
    }

    // Unbind the OnComponentEndOverlap event if it is bound.
    if (AreaCollision->OnComponentEndOverlap.IsBound())
    {
        AreaCollision->OnComponentEndOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionEndOverlap);  // Remove the OnCollisionEndOverlap event binding.
    }

    // Unbind the OnComponentHit event if it is bound.
    if (WeaponMesh->OnComponentHit.IsBound())
    {
        WeaponMesh->OnComponentHit.RemoveDynamic(this, &AWeaponModule::OnWeaponHit);  // Remove the OnWeaponHit event binding.
    }
}

// Function to handle the state when the weapon is in the player's inventory.
void AWeaponModule::HandleInventoryState()
{
    // Configure WeaponMesh for inventory state:
    // Disable collision, physics simulation, and gravity.
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for WeaponMesh.
    WeaponMesh->SetSimulatePhysics(false);  // Disable physics simulation for WeaponMesh.
    WeaponMesh->SetEnableGravity(false);  // Disable gravity for WeaponMesh.

    // Configure AreaCollision for inventory state:
    // Disable collision.
    AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for AreaCollision.

    // Unbind the OnComponentBeginOverlap event if it is bound.
    if (AreaCollision->OnComponentBeginOverlap.IsBound())
    {
        AreaCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionOverlap);  // Remove the OnCollisionOverlap event binding.
    }

    // Unbind the OnComponentEndOverlap event if it is bound.
    if (AreaCollision->OnComponentEndOverlap.IsBound())
    {
        AreaCollision->OnComponentEndOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionEndOverlap);  // Remove the OnCollisionEndOverlap event binding.
    }

    // Unbind the OnComponentHit event if it is bound.
    if (WeaponMesh->OnComponentHit.IsBound())
    {
        WeaponMesh->OnComponentHit.RemoveDynamic(this, &AWeaponModule::OnWeaponHit);  // Remove the OnWeaponHit event binding.
    }
}

// Function to handle the state when the weapon is stored in a safe place.
void AWeaponModule::HandleStorageState()
{
    // Configure WeaponMesh for storage state:
    // Disable collision, physics simulation, and gravity.
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for WeaponMesh.
    WeaponMesh->SetSimulatePhysics(false);  // Disable physics simulation for WeaponMesh.
    WeaponMesh->SetEnableGravity(false);  // Disable gravity for WeaponMesh.

    // Configure AreaCollision for storage state:
    // Disable collision.
    AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for AreaCollision.

    // Unbind the OnComponentBeginOverlap event if it is bound.
    if (AreaCollision->OnComponentBeginOverlap.IsBound())
    {
        AreaCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionOverlap);  // Remove the OnCollisionOverlap event binding.
    }

    // Unbind the OnComponentEndOverlap event if it is bound.
    if (AreaCollision->OnComponentEndOverlap.IsBound())
    {
        AreaCollision->OnComponentEndOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionEndOverlap);  // Remove the OnCollisionEndOverlap event binding.
    }

    // Unbind the OnComponentHit event if it is bound.
    if (WeaponMesh->OnComponentHit.IsBound())
    {
        WeaponMesh->OnComponentHit.RemoveDynamic(this, &AWeaponModule::OnWeaponHit);  // Remove the OnWeaponHit event binding.
    }
}

// Function to handle the state when the weapon is given as a reward.
void AWeaponModule::HandleRewardState()
{
    // Configure WeaponMesh for reward state:
    // Disable collision, physics simulation, and gravity.
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for WeaponMesh.
    WeaponMesh->SetSimulatePhysics(false);  // Disable physics simulation for WeaponMesh.
    WeaponMesh->SetEnableGravity(false);  // Disable gravity for WeaponMesh.

    // Configure AreaCollision for reward state:
    // Disable collision.
    AreaCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Disable collision for AreaCollision.

    // Unbind the OnComponentBeginOverlap event if it is bound.
    if (AreaCollision->OnComponentBeginOverlap.IsBound())
    {
        AreaCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionOverlap);  // Remove the OnCollisionOverlap event binding.
    }

    // Unbind the OnComponentEndOverlap event if it is bound.
    if (AreaCollision->OnComponentEndOverlap.IsBound())
    {
        AreaCollision->OnComponentEndOverlap.RemoveDynamic(this, &AWeaponModule::OnCollisionEndOverlap);  // Remove the OnCollisionEndOverlap event binding.
    }

    // Unbind the OnComponentHit event if it is bound.
    if (WeaponMesh->OnComponentHit.IsBound())
    {
        WeaponMesh->OnComponentHit.RemoveDynamic(this, &AWeaponModule::OnWeaponHit);  // Remove the OnWeaponHit event binding.
    }
}
#pragma endregion
#pragma region VisualFxs
#pragma endregion
#pragma region SoundFxs
void AWeaponModule::Play2DSound(USoundCue* InSoundCue)
{
    // Check if the provided sound cue is valid (not null).
    if (InSoundCue != nullptr)
    {
        // Play the sound cue in 2D (non-spatialized) using the gameplay statics utility.
        UGameplayStatics::PlaySound2D
        (
            GetWorld(), // Get the current game world.
            InSoundCue  // The sound cue to play.
        );
    }
    else
    {
        LOG_INVALID(SoundCue)
    }
}
#pragma endregion