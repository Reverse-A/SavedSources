// // @2023 All rights reversed by Reverse-Alpha Studios

#include "MeleeWeapon.h"

#include "Ascend/Tools/LogMacros.h"

#include "Ascend/AI/AIEntityModule.h"
#include "Ascend/AI/Actors/Humanoid/AIMeleeHumanoidModule.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


static FName NAME_DamageCollision(TEXT("Damage Collision"));

#pragma region Main
AMeleeWeapon::AMeleeWeapon()
{
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(NAME_DamageCollision);
	DamageCollision->SetupAttachment(WeaponMesh);

	DamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SetWeaponType(EWeaponType::EWT_Melee);
	FVector BoxExtent(2.0f, 2.0f, 40.0f);
	DamageCollision->SetBoxExtent(BoxExtent);
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("MeleeWeapon");

	MeleeWeaponProperties.UpdateWeaponProperties
	(
		MeleeWeaponProperties.GetMeleeWeaponType(),
		MeleeWeaponProperties.GetRarityCategory()
	);

	InitialMeleeWeaponProperties();
}

void AMeleeWeapon::InitialMeleeWeaponProperties()
{
	FName ItemName;
	FString ItemDescription;

	if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::Sword)
	{
		FVector BoxExtent(2.0f, 2.0f, 40.0f);
		DamageCollision->SetBoxExtent(BoxExtent);
		SetInventoryItemImage(MeleeWeaponImages.GetSwordImage());
		if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Sword (Common)";
			ItemDescription = "A common sword.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Sword (Rare)";
			ItemDescription = "A rare sword with unique craftsmanship.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Sword (Epic)";
			ItemDescription = "An epic sword imbued with mystical powers.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Sword (Unique)";
			ItemDescription = "A one-of-a-kind sword, legendary among blades.";
		}
	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::Katana)
	{
		FVector BoxExtent(2.0f, 2.0f, 40.0f);
		DamageCollision->SetBoxExtent(BoxExtent);
		SetInventoryItemImage(MeleeWeaponImages.GetKatanaImage());

		if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Katana (Common)";
			ItemDescription = "A common katana.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Katana (Rare)";
			ItemDescription = "A rare katana with unique craftsmanship.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Katana (Epic)";
			ItemDescription = "An epic katana imbued with mystical powers.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Katana (Unique)";
			ItemDescription = "A one-of-a-kind katana, legendary among blades.";
		}
	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::Axe)
	{
		FVector BoxExtent(2.0f, 2.0f, 40.0f);
		DamageCollision->SetBoxExtent(BoxExtent);
		SetInventoryItemImage(MeleeWeaponImages.GetAxeImage());

		if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Axe (Common)";
			ItemDescription = "A common axe.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Axe (Rare)";
			ItemDescription = "A rare axe with unique craftsmanship.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Axe (Epic)";
			ItemDescription = "An epic axe imbued with mystical powers.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Axe (Unique)";
			ItemDescription = "A one-of-a-kind axe, legendary among weapons.";
		}
	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::Mace)
	{
		FVector BoxExtent(2.0f, 2.0f, 40.0f);
		DamageCollision->SetBoxExtent(BoxExtent);
		SetInventoryItemImage(MeleeWeaponImages.GetMaceImage());

		if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Mace (Common)";
			ItemDescription = "A common mace.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Mace (Rare)";
			ItemDescription = "A rare mace with unique craftsmanship.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Mace (Epic)";
			ItemDescription = "An epic mace imbued with mystical powers.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Mace (Unique)";
			ItemDescription = "A one-of-a-kind mace, legendary among weapons.";
		}
	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::Dagger)
	{
		FVector BoxExtent(2.0f, 2.0f, 40.0f);
		DamageCollision->SetBoxExtent(BoxExtent);

		SetInventoryItemImage(MeleeWeaponImages.GetDaggerImage());
		if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Dagger (Common)";
			ItemDescription = "A common dagger.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Dagger (Rare)";
			ItemDescription = "A rare dagger with unique craftsmanship.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Dagger (Epic)";
			ItemDescription = "An epic dagger imbued with mystical powers.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Dagger (Unique)";
			ItemDescription = "A one-of-a-kind dagger, legendary among weapons.";
		}
	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::Club)
	{
		FVector BoxExtent(2.0f, 2.0f, 40.0f);
		DamageCollision->SetBoxExtent(BoxExtent);

		SetInventoryItemImage(MeleeWeaponImages.GetClubImage());

		if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Club (Common)";
			ItemDescription = "A common club.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Club (Rare)";
			ItemDescription = "A rare club with unique craftsmanship.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Club (Epic)";
			ItemDescription = "An epic club imbued with mystical powers.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Club (Unique)";
			ItemDescription = "A one-of-a-kind club, legendary among weapons.";
		}
	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::Spear)
	{
		FVector BoxExtent(2.0f, 2.0f, 40.0f);
		DamageCollision->SetBoxExtent(BoxExtent);

		SetInventoryItemImage(MeleeWeaponImages.GetSpearImage());

		if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Spear (Common)";
			ItemDescription = "A common spear.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Spear (Rare)";
			ItemDescription = "A rare spear with unique craftsmanship.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Spear (Epic)";
			ItemDescription = "An epic spear imbued with mystical powers.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Spear (Unique)";
			ItemDescription = "A one-of-a-kind spear, legendary among weapons.";
		}
	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::Shield)
	{
		FVector BoxExtent(2.0f, 2.0f, 40.0f);
		DamageCollision->SetBoxExtent(BoxExtent);

		SetInventoryItemImage(MeleeWeaponImages.GetShieldImage());

		if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Shield (Common)";
			ItemDescription = "A common shield.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Shield (Rare)";
			ItemDescription = "A rare shield with unique craftsmanship.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Shield (Epic)";
			ItemDescription = "An epic shield imbued with mystical powers.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Shield (Unique)";
			ItemDescription = "A one-of-a-kind shield, legendary among defenses.";
		}
	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::HiddenBlade)
	{
		FVector BoxExtent(2.0f, 2.0f, 40.0f);
		DamageCollision->SetBoxExtent(BoxExtent);

		SetInventoryItemImage(MeleeWeaponImages.GetHiddenBladeImage());

		if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Common)
		{
			ItemName = "Hidden Blade (Common)";
			ItemDescription = "A common hidden blade.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Rare)
		{
			ItemName = "Hidden Blade (Rare)";
			ItemDescription = "A rare hidden blade with unique craftsmanship.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Epic)
		{
			ItemName = "Hidden Blade (Epic)";
			ItemDescription = "An epic hidden blade imbued with mystical powers.";
		}
		else if (MeleeWeaponProperties.GetRarityCategory() == ERarityCategory::Unique)
		{
			ItemName = "Hidden Blade (Unique)";
			ItemDescription = "A one-of-a-kind hidden blade, legendary among assassins.";
		}
	}
	float Price = MeleeWeaponProperties.GetPrice();

	SetInventoryItemName(ItemName);
	SetInventoryItemPrice(Price);
	SetInventoryItemDescription(ItemDescription);
}

#pragma endregion

#pragma region Weapon-Overlaps
// Function to handle damage by collision
void AMeleeWeapon::OnBeginOverlapDamage
(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// Check if the overlapping actor is the owner of this weapon or if the weapon is currently dealing damage
	if (OtherActor == GetOwner())
	{
		// Do not proceed if the overlapping actor is the owner or if the weapon is currently dealing damage
		return;
	}

	// Check if the overlapping component is valid and not simulating physics
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		// Do no proceed if the overlapping component is simulating physics
		return;
	}

	if (bMeleeWeaponDamageAgain)
	{
		// Get the current melee weapon's constant weapon damage and penetration value from the weapon's properties
		float const WeaponDamage = MeleeWeaponProperties.GetDamageValue();
		float const WeaponPenetration = MeleeWeaponProperties.GetPenetrationValue();

		// Calculate the durability damage by dividing the total damage by a fixed divisor
		float DurabilityDamage = WeaponDamage;

		// Get the current durability value of the melee weapon
		float CurrentDurability = MeleeWeaponProperties.GetDurability();


		if (OtherActor && OtherActor->IsA<ABaseCharacter>() && AIEntity)
		{
			BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(OtherActor) : BaseCharacter;
			if (BaseCharacter)
			{
				BaseCharacter->ReceiveDamage(WeaponDamage);
				BaseCharacter = nullptr;
			}
			bMeleeWeaponDamageAgain = false;
		}
		else if (OtherActor && OtherActor->IsA<AAIEntityModule>() && BaseCharacter)
		{
			AIEntity = AIEntity == nullptr ? Cast<AAIEntityModule>(OtherActor) : AIEntity;
			if (AIEntity)
			{
				UGameplayStatics::ApplyDamage
				(
					AIEntity,
					WeaponDamage,
					BaseCharacter->GetInstigatorController(),
					this,
					UDamageType::StaticClass()
				);
				AIEntity = nullptr;
			}
			// Update the durability property by subtracting the calculated durability damage
			// This simulates wear and tear on the weapon due to its usage
			MeleeWeaponProperties.SetDurability(CurrentDurability - DurabilityDamage);
			bMeleeWeaponDamageAgain = false;
		}

		// TODO:
		// In the future, based on durability values, the item can be unusable, breakable, 
		// or its physical damage or any multiplier can be reduced.
	}

#define CONTROL_PAWNS 0
#if CONTROL_PAWNS == 1
	if (BaseCharacter)
	{
		// Debug message to confirm that BaseCharacter is valid
		UE_LOG(LogTemp, Warning, TEXT("BaseCharacter is valid"));
	}
	else
	{
		// Debug message if BaseCharacter is nullptr
		UE_LOG(LogTemp, Warning, TEXT("BaseCharacter is nullptr"));
	}

	if (AIEntity)
	{
		// Debug message to confirm that AIEntity is valid
		UE_LOG(LogTemp, Warning, TEXT("AIEntity is valid"));
	}
	else
	{
		// Debug message if AIEntity is nullptr
		UE_LOG(LogTemp, Warning, TEXT("AIEntity is nullptr"));
	}

	if (OtherActor && OtherActor->IsA<ABaseCharacter>())
	{
		UE_LOG(LogTemp, Warning, TEXT("IsABaseCharacter"));
	}
	else if (OtherActor && OtherActor->IsA<AAIEntityModule>() && BaseCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("IsABaseCharacter"));
	}
#endif
}

void AMeleeWeapon::OverlapDamageEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlappedComp	= nullptr;
	OtherActor		= nullptr;
	OtherComp		= nullptr;
}

void AMeleeWeapon::DeActivateDamageCollision()
{
	if (DamageCollision->OnComponentBeginOverlap.IsBound())
	{
		DamageCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AMeleeWeapon::OverlapDamageEnd);
		bMeleeWeaponDamageAgain = false;
		DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		LW("RemovedDynamic DamageCollision")

	}	
}

void AMeleeWeapon::ActivateWeaponOverlapDynamics(bool bActivate)
{
	if (bActivate)
	{
		if (DamageCollision->OnComponentBeginOverlap.IsBound() == false)
		{
			DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnBeginOverlapDamage);
		}
	}
	else
	{
		if (DamageCollision->OnComponentBeginOverlap.IsBound())
		{
			DamageCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AMeleeWeapon::OverlapDamageEnd);
			bMeleeWeaponDamageAgain = true;
		}
	}
}

#pragma endregion
