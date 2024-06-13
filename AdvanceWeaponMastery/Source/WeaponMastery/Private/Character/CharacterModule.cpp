#include "Character/CharacterModule.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include <WeaponMastery/Public/Actor/WeaponModule.h>
#include <WeaponMastery/Public/Actor/ShortRangeWeaponModule.h>
#include <WeaponMastery/Public/Actor/LongRangeWeaponModule.h>

#include <WeaponMastery/Public/Component/ArsenalComponent.h>
#include <WeaponMastery/Public/Component/CharacterUIComponent.h>

static FName NAME_ArsenalComponent(TEXT("ArsenalSystemComponent"));
static FName NAME_UIComponent(TEXT("CharacterUserInterfaceComponent"));

ACharacterModule::ACharacterModule()
{
	// Initialize capsule size
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create and attach first person camera component
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Configure mesh visibility and attachment
	GetMesh()->SetOnlyOwnerSee(true);
	GetMesh()->SetupAttachment(FirstPersonCameraComponent);
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;
	GetMesh()->SetRelativeLocation(FVector(-30.0f, 0.f, -150.f));

	// Create arsenal component and UI component
	ArsenalComponent = CreateDefaultSubobject<UArsenalComponent>(NAME_ArsenalComponent);
	CharacterUIComponent = CreateDefaultSubobject<UCharacterUIComponent>(NAME_UIComponent);
}

// Called when the game starts or when spawned
void ACharacterModule::BeginPlay()
{
	Super::BeginPlay(); // Call the parent class's BeginPlay function
}

// Called every frame
void ACharacterModule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Call the parent class's Tick function
	PerformRayCast(); // Perform ray cast for crosshair
}

// Called before components are initialized
void ACharacterModule::PreInitializeComponents()
{
	Super::PreInitializeComponents(); // Call the parent class's PreInitializeComponents function

	// Initialize arsenal component if available
	if (ArsenalComponent != nullptr)
	{
		ArsenalComponent->InitializeCharacterReference(this);
	}

	// Initialize character UI component if available
	if (CharacterUIComponent != nullptr)
	{
		CharacterUIComponent->InitializeCharacterReference(this);
	}
}



void ACharacterModule::PerformRayCast()
{
	// This function performs a ray cast from the player's camera to detect interactable objects in the game world. 
	// It retrieves the camera location and rotation, then casts a ray in the direction the camera is facing. 
	// If the ray hits an object, it checks if the object is a melee weapon, a ranged weapon, or neither. 
	// Depending on the type of object hit, it updates the corresponding weapon information widget with the properties of the detected weapon.
	// If no object is hit, it hides any visible weapon information widgets to maintain a clean UI. 
	// Additionally, it draws a debug sphere at the point of impact to visually indicate where the ray hits, 
	// aiding in debugging and visualization of the interaction.

	// Retrieve the camera location and rotation of the player.
	FVector CameraLocation;
	FRotator CameraRotation;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	}

	// Calculate the start and end points of the ray cast.
	FVector Start = CameraLocation;
	FVector End = Start + (CameraRotation.Vector() * 1000.0f); // Adjust the range as needed

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);  // Ignore the character itself

	// Perform the line trace to detect objects in the scene.
	bool bHit = GetWorld()->LineTraceSingleByChannel
	(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		CollisionParams
	);

	// Draw a debug sphere at the hit location to visualize the ray cast.
	// The purpose of this debug sphere is to determine and display the exact location of the impact point, which is the final point of LineTrace. 
	// Its only purpose is to be an example like a crosshair.
	DrawDebugSphere
	(
		GetWorld(),
		HitResult.ImpactPoint,
		3.0f,
		1,
		FColor::Red,
		false,
		0
	);

	// Check if an object was hit
	if (bHit)
	{
		// This code snippet checks if the hit actor is a weapon module and if it's within a certain distance for pickup.
		if (HitResult.GetActor()->IsA<AWeaponModule>())
		{
			AWeaponModule* Weapon = Cast<AWeaponModule>(HitResult.GetActor());

			if (Weapon != nullptr)
			{
				// If the hit actor is a weapon module and is further than 250 units away, return without further action.
				if (HitResult.Distance > 250.0f)
				{
					return;
				}
				// If the hit actor is a weapon module and is within 250 units, update the pending pickup weapon in the ArsenalSystemComponent.
				else
				{
					ArsenalComponent->UpdatePendingPickupWeapon(Weapon);
				}
			}		
		}
		else
		{
			// If the hit actor is not a weapon module, reset the pending pickup weapon in the ArsenalSystemComponent.
			if (ArsenalComponent->GetPendingPickupWeapon() != nullptr)
			{
				ArsenalComponent->UpdatePendingPickupWeapon(nullptr);
			}
		}

		// Check the type of object hit and update the corresponding weapon information widget
		if (HitResult.GetActor()->IsA<AShortRangeWeaponModule>())
		{
			// If the hit object is a melee weapon module, cast it to AMeleeWeaponModule and update the widget with its information
			AShortRangeWeaponModule* ShortRangeWeapon = Cast<AShortRangeWeaponModule>(HitResult.GetActor());
			if (ShortRangeWeapon != nullptr)
			{
				if (CharacterUIComponent != nullptr)
				{
					CharacterUIComponent->UpdateShortRangeWeaponInformationWidget(ShortRangeWeapon);
				}
			}
		}
		else if (HitResult.GetActor()->IsA<ALongRangeWeaponModule>())
		{
			// If the hit object is a ranged weapon module, cast it to ARangeWeaponModule and update the widget with its information
			ALongRangeWeaponModule* LongRangeWeapon = Cast<ALongRangeWeaponModule>(HitResult.GetActor());
			if (LongRangeWeapon != nullptr)
			{
				if (CharacterUIComponent != nullptr)
				{
					CharacterUIComponent->UpdateLongRangeWeaponInformationWidget(LongRangeWeapon);
				}
			}
		}
		else
		{
			// If the hit object is neither a melee nor a ranged weapon, hide any visible weapon information widgets
			if (CharacterUIComponent != nullptr)
			{
				CharacterUIComponent->HideVisibleWeaponInformationWidgets();
			}
		}
	}
	else
	{
		if (ArsenalComponent->GetPendingPickupWeapon() != nullptr)
		{
			ArsenalComponent->UpdatePendingPickupWeapon(nullptr);
		}

		// If no object was hit, hide any visible weapon information widgets
		if (CharacterUIComponent != nullptr)
		{
			CharacterUIComponent->HideVisibleWeaponInformationWidgets();
		}
	}
}