// @2023 All rights reversed by Reverse-Alpha Studios


#include "EffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "QuantumWarfare/Ability/AbilitySystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

FName NAME_EffectActorMesh(TEXT("Mesh"));
FName NAME_EffectActorSphere(TEXT("Sphere"));


AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC == nullptr) return;

	check(InGameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = 
	TargetASC->MakeOutgoingSpec
	(
		InGameplayEffectClass,
		ActorLevel,
		EffectContextHandle
	);

	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf
	(
		*EffectSpecHandle.Data.Get()
	);

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}

	PlaySoundToPlayer(EffectSound);
}

void AEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	else if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	else if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	else if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	else if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

		if (!IsValid(TargetASC)) 
			return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

void AEffectActor::PlaySoundToPlayer(USoundCue* InSoundCue)
{
	if (EffectSound != nullptr && bIsSoundPlaying == false)
	{		// Play the 2D sound
		UGameplayStatics::PlaySound2D(GetWorld(), InSoundCue);

		bIsSoundPlaying = true;
		GetWorldTimerManager().SetTimer(TimerHandle_ResetSoundFlag, this, &AEffectActor::ResetSoundFlag, 2.0f, false);
	}
}

void AEffectActor::ResetSoundFlag()
{
	bIsSoundPlaying = false; // Reset the flag after the delay
}




#pragma region OldProgress
//AEffectActor::AEffectActor()
//{
//	PrimaryActorTick.bCanEverTick = false;
//
//	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(NAME_EffectActorMesh);
//	SetRootComponent(Mesh);
//
//	Sphere = CreateDefaultSubobject<USphereComponent>(NAME_EffectActorSphere);
//	Sphere->SetupAttachment(GetRootComponent());
//}
//
//void AEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	//TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack!
//	if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
//	{
//		const UAttributeSystem* AttributeSystem = Cast<UAttributeSystem>
//		(AbilitySystemInterface->GetAbilitySystemComponent()->GetAttributeSet(UAttributeSystem::StaticClass()));
//
//		UAttributeSystem* MutableAttributeSystem = const_cast<UAttributeSystem*>(AttributeSystem);
//		MutableAttributeSystem->SetHealth(AttributeSystem->GetHealth() + 25.0f);
//		MutableAttributeSystem->SetShield(AttributeSystem->GetShield() + 25.0f);
//		MutableAttributeSystem->SetEnergy(AttributeSystem->GetEnergy() + 25.0f);
//		MutableAttributeSystem->SetStamina(AttributeSystem->GetStamina() + 25.0f);
//
//		Destroy();
//	}
//}
//
//void AEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//
//}
//void AEffectActor::BeginPlay()
//{
//	Super::BeginPlay();
//
//	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnOverlap);
//	Sphere->OnComponentEndOverlap.AddDynamic(this, &AEffectActor::EndOverlap);
//
//}
//
//
//void AEffectActor::BeginPlay()
//{
//	Super::BeginPlay();
//	
//	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnOverlap);
//	Sphere->OnComponentEndOverlap.AddDynamic(this, &AEffectActor::EndOverlap);
//
//}
#pragma endregion

