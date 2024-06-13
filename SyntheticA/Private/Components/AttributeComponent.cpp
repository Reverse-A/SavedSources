// @2023 All rights reversed by Reverse-Alpha Studios


#include "Components/AttributeComponent.h"
#include "Components/EnhancedMovementComponent.h"
#include "Characters/BaseCharacter.h"


#pragma region Initialization
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HealRampUp(DeltaTime);
	ShieldRampUp(DeltaTime);
}
#pragma endregion

#pragma region Heal++
void UAttributeComponent::Healing(float HealAmount, float HealingTime)
{
	bHealing = true;
	HealingRate = HealAmount / HealingTime;
	AmountToHeal += HealAmount;

}

void UAttributeComponent::HealRampUp(float DeltaTime)
{
	bool ReturnTypes =
		bHealing == false		 ||
		BaseCharacter == nullptr ||
		BaseCharacter->GetPlayerIsEliminated() == true;

	if (ReturnTypes) return;

	const float RefreshingFrame = HealingRate * DeltaTime;

	float CharacterHealth    = BaseCharacter->GetHealth();
	float CharacterMaxHealth = BaseCharacter->GetMaxHealth();

	float IncreaseClampedHealth = FMath::Clamp(CharacterHealth + RefreshingFrame, 0, CharacterMaxHealth);
	BaseCharacter->SetHealth(IncreaseClampedHealth);
	BaseCharacter->UpdateHUDHealth(false);
	AmountToHeal -= RefreshingFrame;

	if (AmountToHeal <= 0.0f || CharacterHealth >= CharacterMaxHealth)
	{
		bHealing = false;
		AmountToHeal = 0.0f;
	}
}

#pragma endregion
#pragma region Shield++
void UAttributeComponent::RegenerationShield(float ShieldAmount, float RefreshingTime)
{
	bShieldRefreshing = true;

	ShieldRefreshingRate = ShieldAmount / RefreshingTime;
	ShieldRefreshingAmount += ShieldAmount;
}
void UAttributeComponent::ShieldRampUp(float DeltaTime)
{
	bool ReturnTypes =
		bShieldRefreshing == false ||
		BaseCharacter == nullptr ||
		BaseCharacter->GetPlayerIsEliminated() == true;

	if (ReturnTypes) return;

	const float RefreshingFrame = ShieldRefreshingRate * DeltaTime;

	float CharacterShield = BaseCharacter->GetShield();
	float CharacterMaxShield = BaseCharacter->GetMaxShield();

	float IncreaseClampedShield = FMath::Clamp(CharacterShield + RefreshingFrame, 0, CharacterMaxShield);
	BaseCharacter->SetShield(IncreaseClampedShield);

	BaseCharacter->UpdateHUDShield(false);
	ShieldRefreshingAmount -= RefreshingFrame;

	if (ShieldRefreshingAmount <= 0.0f || CharacterShield >= CharacterMaxShield)
	{
		bShieldRefreshing	= false;
		ShieldRefreshingAmount = 0.0f;
	}
}
#pragma endregion
#pragma region Speed++
void UAttributeComponent::SetInitialSpeed(float WalkSpeed, float CrouchSpeed)
{
	InitialWalkSpeed = WalkSpeed;
	InitialCrouchSpeed = CrouchSpeed;
}



void UAttributeComponent::SpeedIncrement(float IncreaseWalkSpeed, float IncreaseCrouchedSpeed, float IncreaseSpeedTime)
{
	bool ReturnTypes =
		BaseCharacter == nullptr ||
		BaseCharacter->GetEnhancedMovementComponent() == nullptr ||
		BaseCharacter->GetHealth() <= 0;

	if (ReturnTypes) return;

	BaseCharacter->GetWorldTimerManager().SetTimer
	(
		SpeedIncreamentTimerHandler,
		this,
		&UAttributeComponent::ResetSpeedIncreament,
		IncreaseSpeedTime
	);

	if (BaseCharacter->GetEnhancedMovementComponent())
	{
		BaseCharacter->GetEnhancedMovementComponent()->MaxWalkSpeed = IncreaseWalkSpeed;
		BaseCharacter->GetEnhancedMovementComponent()->MaxWalkSpeedCrouched = IncreaseCrouchedSpeed;
	}

	Multicast_SpeedIncreament(IncreaseWalkSpeed, IncreaseCrouchedSpeed);
}

void UAttributeComponent::Multicast_SpeedIncreament_Implementation(float BaseWalkSpeed, float BaseCrouchedSpeed)
{
	if (BaseCharacter && BaseCharacter->GetEnhancedMovementComponent())
	{
		BaseCharacter->GetEnhancedMovementComponent()->MaxWalkSpeed = BaseWalkSpeed;
		BaseCharacter->GetEnhancedMovementComponent()->MaxWalkSpeedCrouched = BaseCrouchedSpeed;
	}

}

void UAttributeComponent::ResetSpeedIncreament()
{
	bool ReturnTypes =
		BaseCharacter == nullptr ||
		BaseCharacter->GetEnhancedMovementComponent() == nullptr ||
		BaseCharacter->GetHealth() <= 0;

	if (ReturnTypes) return;

	BaseCharacter->GetEnhancedMovementComponent()->MaxWalkSpeed			= InitialWalkSpeed;
	BaseCharacter->GetEnhancedMovementComponent()->MaxWalkSpeedCrouched = InitialCrouchSpeed;

	Multicast_SpeedIncreament(InitialWalkSpeed, InitialCrouchSpeed);
}
#pragma endregion
#pragma region Jump++
void UAttributeComponent::JumpIncrement(float IncreaseJumpVelocity, float IncreaseJumpTime)
{
	bool ReturnTypes =
		BaseCharacter == nullptr ||
		BaseCharacter->GetEnhancedMovementComponent() == nullptr ||
		BaseCharacter->GetHealth() <= 0;

	if (ReturnTypes) return;

	BaseCharacter->GetWorldTimerManager().SetTimer
	(
		JumpIncreamentTimerHandler,
		this,
		&UAttributeComponent::ResetJumpIncrement,
		IncreaseJumpTime
	);

	if (BaseCharacter->GetEnhancedMovementComponent())
	{
		BaseCharacter->GetEnhancedMovementComponent()->JumpZVelocity = IncreaseJumpVelocity;
	}
	Multicast_JumpIncrement(IncreaseJumpVelocity);
}
void UAttributeComponent::Multicast_JumpIncrement_Implementation(float IncreaseJumpVelocity)
{
	if (BaseCharacter && BaseCharacter->GetEnhancedMovementComponent())
	{
		BaseCharacter->GetEnhancedMovementComponent()->JumpZVelocity = IncreaseJumpVelocity;
	}
}
void UAttributeComponent::ResetJumpIncrement()
{
	bool ReturnTypes =
		BaseCharacter == nullptr ||
		BaseCharacter->GetEnhancedMovementComponent() == nullptr ||
		BaseCharacter->GetHealth() <= 0;

	if (ReturnTypes) return;

	if (BaseCharacter->GetEnhancedMovementComponent())
	{
		BaseCharacter->GetEnhancedMovementComponent()->JumpZVelocity = InitialJumpVelocity;
	}
	Multicast_JumpIncrement(InitialJumpVelocity);

}

void UAttributeComponent::SetInitialJumpVelocity(float JumpVelocity)
{
	InitialJumpVelocity = JumpVelocity;
}
#pragma endregion