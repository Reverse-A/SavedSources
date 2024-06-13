// @2023 All rights reversed by Reverse-Alpha Studios


#include "EnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include <QuantumWarfare/Character/AI/AIController/AIControllerModule.h>

#include <QuantumWarfare/UI/Widget/PlayerUserWidget.h>
#include <QuantumWarfare/Ability/AbilitySystem.h>
#include <QuantumWarfare/Ability/AttributeSystem.h>
#include <QuantumWarfare/Ability/BlueprintLibrary/PlayerAbilitySystemLibrary.h>
#include <QuantumWarfare/QWGameplayTags.h>


AEnemyCharacter::AEnemyCharacter()
{
	// Check if the Mesh component is valid
	if (GetMesh() != nullptr)
	{
		GetMesh()->SetCollisionProfileName("Custom");
		GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystem>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAttributeSystem>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;

	AIControllerModule = Cast<AAIControllerModule>(NewController);

	AIControllerModule->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AIControllerModule->RunBehaviorTree(BehaviorTree);
}

void AEnemyCharacter::HighlightActor()
{
	if (GetMesh() != nullptr)
	{
		GetMesh()->SetRenderCustomDepth(true);
	}

	//if (ArsenalComponent->GetEquippedWeapon() != nullptr)
	//{
	//	ArsenalComponent->SetEquippedWeaponRenderCustomDepth(true);
	//}
}

void AEnemyCharacter::UnHighlightActor()
{
	if (GetMesh() != nullptr)
	{
		GetMesh()->SetRenderCustomDepth(false);
	}

	//if (ArsenalComponent->GetEquippedWeapon() != nullptr)
	//{
	//	ArsenalComponent->SetEquippedWeaponRenderCustomDepth(false);
	//}
}

int32 AEnemyCharacter::GetPlayerLevel()
{
	return Level;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	if (HasAuthority())
	{
		UPlayerAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	}

	if (UPlayerUserWidget* PlayerUserWidget = Cast<UPlayerUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		PlayerUserWidget->SetWidgetController(this);
	}

	if (const UAttributeSystem* AttributeSystem = Cast<UAttributeSystem>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSystem->GetHealthAttribute()).AddLambda
		(
			[this] (const FOnAttributeChangeData& Data) 
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSystem->GetMaxHealthAttribute()).AddLambda
		(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FQWGameplayTags::Get().Effects_HitReaction, EGameplayTagEventType::NewOrRemoved).AddUObject
		(
			this,
			&AEnemyCharacter::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(AttributeSystem->GetHealth());
		OnMaxHealthChanged.Broadcast(AttributeSystem->GetMaxHealth());
	}
}

void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AEnemyCharacter::Death()
{
	bDeath = true;
	// TODO : Add Dissolve material and functionalities for enemy's mesh

	/*Temporary uses*/	
	SetLifeSpan(5.0f);

	if (ArsenalComponent->GetEquippedWeapon() != nullptr)
	{
		ArsenalComponent->DestroyEnemyWeapon();
	}
	Super::Death();
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAbilitySystem>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	UPlayerAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}



