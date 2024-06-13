// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/MainHUD.h"
#include "HUD/MainOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"


AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CharacterMovement = GetCharacterMovement();
	CharacterMovement->bOrientRotationToMovement = true;
	CharacterMovement->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	CharacterMovement->MaxWalkSpeed = 200.f;

	Tags.Add(FName("EngageableTarget"));
	InitializeMainOverlay();
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);		

	if (IsAlive())
	{
		CharacterEquippedState == ECharacterEquippedState::ECES_Unequipped ? idleState = true : idleState = false;
		idleState == true ? CharacterMovement->MaxWalkSpeed = 150.f : CharacterMovement->MaxWalkSpeed = 450.f;
	}
	else return;

	if (Attributes && MainOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
		MainOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAction(FName("Jump"),   IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"),  IE_Pressed, this, &AMainCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AMainCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &AMainCharacter::Dodge);
}

void AMainCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}


float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void AMainCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void AMainCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AMainCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes&& MainOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		MainOverlay->SetSouls(Attributes->GetSouls());
	}
}

void AMainCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && MainOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		MainOverlay->SetGold(Attributes->GetGold());
	}
}

void AMainCharacter::MoveForward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied && ActionState != EActionState::EAS_Attacking) return;
	if (Controller && (Value != 0.f))
	{
		// Find out whcih way is forward
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}
void AMainCharacter::MoveRight(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied && ActionState != EActionState::EAS_Attacking ) return;
	if (Controller && (Value != 0.f))
	{
		// Find out which way is right 
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}
void AMainCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
} 
void AMainCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMainCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (ActionState != EActionState::EAS_Attacking)
		{
			if (CanDisarm())
			{
				Disarm();
			}
			else if (CanArm())
			{
				Arm();
			}
		}
	}
}
void AMainCharacter::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina()) return;

	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes && MainOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		MainOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}


void AMainCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);

	}
}

bool AMainCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
	CharacterEquippedState != ECharacterEquippedState::ECES_Unequipped;
}
void AMainCharacter::Die_Implementation()
{
	Super::Die_Implementation();

	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

bool AMainCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}
bool AMainCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}
void AMainCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterEquippedState = ECharacterEquippedState::ECES_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}
void AMainCharacter::Attack()
{
	Super::Attack();

	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}
void AMainCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMainCharacter::DodgeEnd()
{
	Super::DodgeEnd();

	ActionState = EActionState::EAS_Unoccupied;
}

void AMainCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}
void AMainCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}
void AMainCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMainCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMainCharacter::InitializeMainOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
		if (MainHUD)
		{
			MainOverlay = MainHUD->GetMainOverLay();
			if (MainOverlay && Attributes)
			{
				MainOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				MainOverlay->SetStaminaBarPercent(1.f);
				MainOverlay->SetGold(0);
				MainOverlay->SetSouls(0);
			}
		}
	}
}
void AMainCharacter::SetHUDHealth()
{
	if (MainOverlay && Attributes)
	{
		MainOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

bool AMainCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}
bool AMainCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterEquippedState == ECharacterEquippedState::ECES_Unequipped && EquippedWeapon;
}
void AMainCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterEquippedState = ECharacterEquippedState::ECES_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}
void AMainCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterEquippedState = ECharacterEquippedState::ECES_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}
bool AMainCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
	CharacterEquippedState != ECharacterEquippedState::ECES_Unequipped;
}

void AMainCharacter::SpeedUp(float Value)
{
	float WalkSpeed = CharacterMovement->MaxWalkSpeed;
	CharacterMovement->MaxWalkSpeed = WalkSpeed * Value;
}
