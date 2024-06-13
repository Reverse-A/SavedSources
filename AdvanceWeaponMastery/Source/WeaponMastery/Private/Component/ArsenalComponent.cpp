#include "Component/ArsenalComponent.h"
#include "Engine/SkeletalMeshSocket.h"

#include <WeaponMastery/Public/Character/CharacterModule.h>
#include <WeaponMastery/Public/Actor/WeaponModule.h>
#include <WeaponMastery/Public/Actor/ShortRangeWeaponModule.h>
#include <WeaponMastery/Public/Actor/LongRangeWeaponModule.h>

#include <WeaponMastery/Public/Data/WeaponStateData.h>
#include <WeaponMastery/Public/Data/AttainmentData.h>

// Sets default values for this component's properties
UArsenalComponent::UArsenalComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UArsenalComponent::InitializeCharacterReference(ACharacterModule* InCharacter)
{
	OwningCharacter = InCharacter;
}


// Called when the game starts
void UArsenalComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UArsenalComponent::EquipPendingPickupWeapon()
{
	// The EquipPendingPickupWeapon function is designed to equip a weapon for a character in a game. 
	// It handles the logic for attaching a weapon to the character's mesh, ensuring that only one weapon is equipped at a time.
	// If a weapon is already equipped, it is detached and dropped before equipping the new weapon.
	// The function checks the type of weapon (melee or ranged) and attaches it to the appropriate socket on the character's mesh.
	// This ensures that the weapon appears in the correct position and orientation on the character model.

	// Check if the necessary components and variables are valid.
	if (PendingPickupWeapon == nullptr || OwningCharacter == nullptr || OwningCharacter->GetMesh() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PendingPickupWeapon, OwningCharacter, or Mesh is null"));
		return;
	}

	// The UnequipAndDropWeapon function ensures proper management of the currently equipped weapon by detaching it from the character,
	// updating its state to 'Dropped', and clearing relevant references.
	// This prevents the player from holding multiple weapons simultaneously,
	// maintains consistent game state, and prepares the system for the next weapon to be equipped.
	// By encapsulating this logic, we enhance code readability, maintainability, and reduce the risk of bugs related to weapon handling.
	UnequipAndDropWeapon();

	// Check if the overlapping weapon is a melee weapon class.
	if (PendingPickupWeapon->IsA<AShortRangeWeaponModule>())
	{
		// Get the socket for attaching a melee weapon.
		const USkeletalMeshSocket* WeaponHandSocket = OwningCharacter->GetMesh()->GetSocketByName(FName("ShortRangeWeaponSocket"));

		// Casts the pending pickup weapon to the AWeaponModule class and assigns it to the currently equipped weapon
		CurrentlyEquippedWeapon = Cast<AWeaponModule>(PendingPickupWeapon);
		// Ensure the weapon was successfully cast.
		if (CurrentlyEquippedWeapon == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("EquippedWeapon not found"));
			return;
		}

		// Update the character's state to indicate a weapon is equipped.
		OwningCharacter->SetWeaponEquipped(true);

		// Ensure the socket for the weapon was found.
		if (WeaponHandSocket == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("WeaponSocket not found"));
			return;
		}

		// Attach the weapon to the socket with specified attachment rules.
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentlyEquippedWeapon->AttachToComponent(OwningCharacter->GetMesh(), AttachmentRules, WeaponHandSocket->SocketName);

		// Update the weapon's state to indicate it is equipped.
		CurrentlyEquippedWeapon->SetWeaponState(EWeaponState::Equipped);

		ConvertShortRangeWeapon(CurrentlyEquippedWeapon);
	}
	else if (PendingPickupWeapon->IsA<ALongRangeWeaponModule>())
	{
		// Get the socket for attaching a ranged weapon.
		const USkeletalMeshSocket* WeaponHandSocket = OwningCharacter->GetMesh()->GetSocketByName(FName("LongRangeWeaponSocket"));

		// Casts the pending pickup weapon to the AWeaponModule class and assigns it to the currently equipped weapon
		CurrentlyEquippedWeapon = Cast<AWeaponModule>(PendingPickupWeapon);
		// Ensure the weapon was successfully cast
		if (CurrentlyEquippedWeapon == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("EquippedWeapon not found"));
			return;
		}

		// Update the character's state to indicate a weapon is equipped.
		OwningCharacter->SetWeaponEquipped(true);

		// Ensure the socket for the weapon was found.
		if (WeaponHandSocket == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("WeaponSocket not found"));
			return;
		}
		// Attach the weapon to the socket with specified attachment rules.
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentlyEquippedWeapon->AttachToComponent(OwningCharacter->GetMesh(), AttachmentRules, WeaponHandSocket->SocketName);

		EWeaponState EquipState = EWeaponState::Equipped;
		CurrentlyEquippedWeapon->SetWeaponState(EquipState);

		ConvertLongRangeWeapon(CurrentlyEquippedWeapon);
	}
}

void UArsenalComponent::UnequipAndDropWeapon()
{
	// If there is already an equipped weapon, detach it and set its state to dropped.
	if (CurrentlyEquippedWeapon != nullptr)
	{
		// Detach the currently equipped weapon from the character and update its state
		CurrentlyEquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//CurrentlyEquippedWeapon->SetWeaponState(EWeaponState::Dropped);
		CurrentlyEquippedWeapon = nullptr;

		// Clear the references to melee and range weapons if they are set
		if (ShortRangeWeapon != nullptr)
		{
			ShortRangeWeapon = nullptr;
		}

		if (LongRangeWeapon != nullptr)
		{
			LongRangeWeapon = nullptr;
		}
	}
}

void UArsenalComponent::ConvertShortRangeWeapon(AWeaponModule* InWeaponModule)
{
	ShortRangeWeapon = Cast<AShortRangeWeaponModule>(InWeaponModule);
	if (ShortRangeWeapon != nullptr)
	{
		ShortRangeWeapons.Add(ShortRangeWeapon);

		int32 ShortRangeWeaponsIndex = ShortRangeWeapons.Num();

		UE_LOG(LogTemp, Warning, TEXT("ShortRangeWeaponsIndex: %d"), ShortRangeWeaponsIndex);
	}
}

void UArsenalComponent::ConvertLongRangeWeapon(AWeaponModule* InWeaponModule)
{
	LongRangeWeapon = Cast<ALongRangeWeaponModule>(InWeaponModule);
	if (LongRangeWeapon != nullptr)
	{
		LongRangeWeapons.Add(LongRangeWeapon);

		int32 LongRangeWeaponsIndex = LongRangeWeapons.Num();

		UE_LOG(LogTemp, Warning, TEXT("LongRangeWeaponsIndex: %d"), LongRangeWeaponsIndex);
	}
}

void UArsenalComponent::ExecuteWeaponThrow()
{
	// The ExecuteWeaponThrow function is designed to handle the logic for throwing the currently equipped weapon in a game.
	// This involves detaching the weapon from the character, setting its physical properties to simulate a throw,
	// and updating the character's state to reflect that no weapon is currently equipped.
	// The function takes into account the type of weapon (melee or ranged) to determine the appropriate socket location
	// from which to throw the weapon.
	LOG_GENGINE("ExecuteWeaponThrown called", "Red");
	// Clear the references to melee and range weapons if they are set
	if (ShortRangeWeapon != nullptr)
	{
		ShortRangeWeapon = nullptr;
	}

	if (LongRangeWeapon != nullptr)
	{
		LongRangeWeapon = nullptr;
	}


	// Check if the necessary components nad variables are valid.
	if (CurrentlyEquippedWeapon == nullptr || OwningCharacter == nullptr || OwningCharacter->GetController() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EquippedWeapon, OwningCharacter, or Controller is null"));
		return;
	}

	// Determine the socket name based on the weapon type.
	FName SocketName;
	if (CurrentlyEquippedWeapon->IsA<AShortRangeWeaponModule>())
	{
		SocketName = FName("ShortRangeWeaponSocket");
	}
	else if (CurrentlyEquippedWeapon->IsA<ALongRangeWeaponModule>())
	{
		SocketName = FName("LongRangeWeaponSocket");
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unknown weapon type"));
		return;
	}

	// Get the socket location from the character's mesh.
	const USkeletalMeshSocket* WeaponSocket = OwningCharacter->GetMesh()->GetSocketByName(SocketName);
	if (WeaponSocket == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon socket not found"));
		return;
	}

	// Get the location of the socket.
	FVector SocketLocation = WeaponSocket->GetSocketLocation(OwningCharacter->GetMesh());

	// Detach the weapon from the character.
	CurrentlyEquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	EWeaponState DroppedState = EWeaponState::Dropped;
	CurrentlyEquippedWeapon->SetWeaponState(DroppedState);

	// Get the forward vector of the character to determine the throw direction.
	FVector ForwardVector = OwningCharacter->GetActorForwardVector();

	// Set the initial position of the thrown weapon slightly in front of the socket location.
	FVector ThrowStartLocation = SocketLocation + ForwardVector * 100.0f; // Adjust the value to set the initial distance.

	// Set the initial velocity for the thrown weapon.
	FVector ThrowVelocity = ForwardVector * 1000.0f; // Adjust the value to set the throw speed.

	// Set the weapon's location and apply the throw velocity
	CurrentlyEquippedWeapon->SetActorLocation(ThrowStartLocation);
	CurrentlyEquippedWeapon->GetWeaponMesh()->SetPhysicsLinearVelocity(ThrowVelocity);



	// Clear the reference to the equipped weapon
	CurrentlyEquippedWeapon = nullptr;

	// Update the character's weapon equipped status
	OwningCharacter->SetWeaponEquipped(false);
}

void UArsenalComponent::UpdatePendingPickupWeapon(AWeaponModule* InOverlapWeapon)
{
	// Sets the pending pickup weapon to the specified weapon module. 
	// This updates the reference to the weapon the player is currently interacting with and may potentially equip.
	PendingPickupWeapon = InOverlapWeapon;
}
