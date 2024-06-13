// @2023 All rights reversed by Reverse-Alpha Studios


#include "CharacterModule.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"

#include <QuantumWarfare/Ability/AbilitySystem.h>
#include <QuantumWarfare/QuantumWarfare.h>
#include <QuantumWarfare/Character/Animation/PCharacterAnimInstance.h>

FName NAME_Weapon(TEXT("Weapon"));
FName NAME_ArsenalComp(TEXT("ArsenalComponent"));

ACharacterModule::ACharacterModule()
{
	PrimaryActorTick.bCanEverTick = false;

	check(GetMesh());
	check(GetCapsuleComponent());

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	ArsenalComponent = CreateDefaultSubobject<UArsenalComponent>(NAME_ArsenalComp);
}

UAbilitySystemComponent* ACharacterModule::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ACharacterModule::BeginPlay()
{
	Super::BeginPlay();
}


void ACharacterModule::InitAbilityActorInfo()
{

}

void ACharacterModule::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (ArsenalComponent)
	{
		ArsenalComponent->SetModuleCharacter(this);
	}

	CharacterAnimation = Cast<UPCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (CharacterAnimation)
	{
		CharacterAnimation->SetCharacterModule(this);
	}
}



void ACharacterModule::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle  ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle		SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACharacterModule::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,		1.0f);
	ApplyEffectToSelf(DefaultSecondaryAttributes,	1.0f);
	ApplyEffectToSelf(DefaultVitalAttributes,		1.0f);
}

void ACharacterModule::AddCharacterAbilities()
{
	UAbilitySystem* AbilitySystemInstance = CastChecked<UAbilitySystem>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	AbilitySystemInstance->AddCharacterAbilities(StartupAbilities);
}

void ACharacterModule::ABPSetWeaponEquipped()
{
	CharacterAnimation->SetABPEquipmentState(EEquipmentState::Equipped);
}

UAnimMontage* ACharacterModule::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}


#pragma region Hit-Death

void ACharacterModule::Death()
{
	Multicast_Death();
}

void ACharacterModule::Multicast_Death_Implementation()
{
	if (GetMesh() != nullptr)
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetEnableGravity(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	}

	if (GetCapsuleComponent() != nullptr)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Dissolve();
}

void ACharacterModule::Dissolve()
{
	UMaterialInstanceDynamic* DynamicBodyMatInst;
	UMaterialInstanceDynamic* DynamicHairMatInst;
	UMaterialInstanceDynamic* DynamicHeadMatInst;

	DynamicBodyMatInst = UMaterialInstanceDynamic::Create(BodyDissolveMaterialInstance, this);
	GetMesh()->SetMaterial(0, DynamicBodyMatInst);

	DynamicHairMatInst = UMaterialInstanceDynamic::Create(HairDissolveMaterialInstance, this);
	GetMesh()->SetMaterial(1, DynamicHairMatInst);

	DynamicHeadMatInst = UMaterialInstanceDynamic::Create(HeadDissolveMaterialInstance, this);
	GetMesh()->SetMaterial(2, DynamicHeadMatInst);

	StartDissolveTimeLine(DynamicBodyMatInst, DynamicHairMatInst, DynamicHeadMatInst);
}
#pragma endregion