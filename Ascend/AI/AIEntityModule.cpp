// @2023 All rights reversed by Reverse-Alpha Studios


#include "AIEntityModule.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

#include "Ascend/Component/AttributeComponent.h"
#include "Ascend/Component/LevelSystemComponent.h"
#include "Ascend/Widget/HealthBar.h"
#include "Ascend/PickupItem/ExperiencePointPickup.h"


AAIEntityModule::AAIEntityModule()
{
	PrimaryActorTick.bCanEverTick = true;

	AICapsuleComponent = GetCapsuleComponent();
	check(AICapsuleComponent);
	SetRootComponent(AICapsuleComponent);

	AICapsuleComponent->InitCapsuleSize(35.0f, 89.95);
	AICapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	AICapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	AIMesh = GetMesh();
	check(AIMesh);
	
	FVector  MeshRelativeLocation = FVector(0.0f, 0.0f, -93.0f);
	FRotator MeshRelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

	AIMesh->SetupAttachment(AICapsuleComponent);
	AIMesh->SetRelativeLocation(MeshRelativeLocation);
	AIMesh->SetRelativeRotation(MeshRelativeRotation);
	AIMesh->SetCastHiddenShadow(false);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	static FName NAME_Attribute(TEXT("Attribute Component"));
	static FName NAME_LevelSystem(TEXT("LevelSystem Component"));

	AIAttributeComponent = CreateDefaultSubobject<UAttributeComponent>(NAME_Attribute);
	LevelSystem = CreateDefaultSubobject<ULevelSystemComponent>(NAME_LevelSystem);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(GetMesh()); 
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f)); 
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarWidgetClassFinder(TEXT("/Game/Blueprints/UI/WBP_HealthBar"));

	if (HealthBarWidgetClassFinder.Succeeded())
	{
		HealthBarWidgetClass = HealthBarWidgetClassFinder.Class;
	}
}

void AAIEntityModule::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (AIAttributeComponent)
	{
		AIAttributeComponent->SetAIEntityModule(this);
	}
}

void AAIEntityModule::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("AIEntity");

	if (HealthBarWidgetClass)
	{
		HealthBarWidget = CreateWidget<UHealthBar>(GetWorld(), HealthBarWidgetClass);
		if (HealthBarWidget)
		{
			HealthBarWidgetComponent->SetWidget(HealthBarWidget);
			HealthBarWidget->SetBarValuePercent(AIAttributeComponent->GetAIHealth() / AIAttributeComponent->GetAIMaxHealth());
		}
	}
}

void AAIEntityModule::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

int AAIEntityModule::MeleeAttack_Implementation()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
	return 0;
}
#pragma region Life-Conditions

#pragma region Damage-System

float AAIEntityModule::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	ReceiveDamage(DamageAmount);
	HealthBarWidget->SetBarValuePercent(AIAttributeComponent->GetAIHealth() / AIAttributeComponent->GetAIMaxHealth());
	return DamageAmount;
}

void AAIEntityModule::ReceiveDamage(float DamageValue)
{
	if (AIAttributeComponent != nullptr)
	{
		if (AIAttributeComponent->GetAIHealth() <= 0)
		{
			return;
		}
		float CurrentHealth = AIAttributeComponent->GetAIHealth();
		float GetMaxHealth	= AIAttributeComponent->GetAIMaxHealth();
		float NewHealth		= CurrentHealth - DamageValue;
		NewHealth			= FMath::Clamp(NewHealth, 0.0f, GetMaxHealth);
		AIAttributeComponent->SetAIHealth(NewHealth);

		if (AIAttributeComponent->GetAIHealth() <= 0)
		{
			Death();
		}
	}
}


#pragma endregion Life-Condition
float AAIEntityModule::AIEntityHealth()
{
	return AIAttributeComponent->GetAIHealth();
}
void AAIEntityModule::Death()
{
	OnDeath();
}


void AAIEntityModule::OnDeath()
{
	if (bNeutralized)
	{
		return;
	}
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpawnExperiencePoints(GetActorLocation());
	bNeutralized = true;

	HealthBarWidget->SetVisibility(ESlateVisibility::Hidden);

	GetWorld()->GetTimerManager().SetTimer(
		DeathTimerHandler,
		this,
		&AAIEntityModule::DeathTimerFinished,
		5.0f,  // 5 seconds delay
		false  // Whether the timer should repeat or not (false means it won't repeat)
	);
}

void AAIEntityModule::DeathTimerFinished()
{
	Destroy();
}

bool AAIEntityModule::IsAIDead()
{
	if (AIAttributeComponent != nullptr)
	{
		if (AIAttributeComponent->GetAIHealth() <= 0)
		{
			return true;
		}
	}
	return false;
}
#pragma endregion Life-Conditions

#pragma region Level-System
float AAIEntityModule::GetExperience()
{
	return 0.0f;
}

#pragma endregion

#pragma region ExperiencePoint
void AAIEntityModule::SpawnExperiencePoints(const FVector& SpawnLocation)
{
	if (ExperiencePointPickupClass)
	{
		FActorSpawnParameters SpawnParameters;

		SpawnParameters.Owner = this;

		GetWorld()->SpawnActor<AExperiencePointPickup>
		(
			ExperiencePointPickupClass,
			SpawnLocation,
			FRotator::ZeroRotator,
			SpawnParameters
		);
	}
}
#pragma endregion