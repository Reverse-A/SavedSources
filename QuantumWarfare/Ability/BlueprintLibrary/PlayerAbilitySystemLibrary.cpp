// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include <QuantumWarfare/UI/HUD/PlayerHUD.h>
#include <QuantumWarfare/UI/WidgetController/PlayerWidgetController.h>
#include <QuantumWarfare/PlayerState/PlayerStateModule.h>
#include <QuantumWarfare/GameMode/BaseGameMode.h>
#include <QuantumWarfare/Ability/AbilitySystemTypes.h>

UPlayerOverlayWidgetController* UPlayerAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PC->GetHUD()))
		{
			APlayerStateModule* PS = PC->GetPlayerState<APlayerStateModule>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return PlayerHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UPlayerAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PC->GetHUD()))
		{
			APlayerStateModule* PS = PC->GetPlayerState<APlayerStateModule>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return PlayerHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UPlayerAbilitySystemLibrary::InitializeDefaultAttributes
(
	const UObject* WorldContextObject, 
	ECharacterClass CharacterClass, 
	float Level,
	UAbilitySystemComponent* AbilitySystemComponent
)
{
	AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

	UCharacterClassInformation* CharacterClassInformation = GetCharacterClassInformation(WorldContextObject);
	FCharacterClassDefaultInformation ClassDefaultInformation = CharacterClassInformation->GetClassDefaultInformation(CharacterClass);

	
	/* Primary Attributes */
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec
	(
		ClassDefaultInformation.PrimaryAttributes,
		Level, 
		PrimaryAttributesContextHandle
	);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());


	/* Secondary Attributes */
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec
	(
		CharacterClassInformation->SecondaryAttributes,
		Level,
		SecondaryAttributesContextHandle
	);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());


	/* Applying Vital Attributes */
	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec
	(
		CharacterClassInformation->VitalAttributes,
		Level,
		VitalAttributesContextHandle
	);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UPlayerAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent)
{
	ABaseGameMode* BaseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (BaseGameMode == nullptr) return;

	UCharacterClassInformation* CharacterClassInformation = GetCharacterClassInformation(WorldContextObject);

	for (TSubclassOf<UGameplayAbility> AbilitiyClass : CharacterClassInformation->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilitiyClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInformation* UPlayerAbilitySystemLibrary::GetCharacterClassInformation(const UObject* WorldContextObject)
{
	ABaseGameMode* BaseGameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (BaseGameMode == nullptr)
	{
		return nullptr;
	}
	return BaseGameMode->CharacterClassInformation;
}

bool UPlayerAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FQWGameplayEffectContext* QW_EffectContext = static_cast<const FQWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return QW_EffectContext->IsBlockedHit();
	}
	return false;
}

bool UPlayerAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FQWGameplayEffectContext* QW_EffectContext = static_cast<const FQWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return QW_EffectContext->IsCriticalHit();
	}
	return false;
}

void UPlayerAbilitySystemLibrary::SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FQWGameplayEffectContext* QW_EffectContext = static_cast<FQWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		QW_EffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UPlayerAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FQWGameplayEffectContext* QW_EffectContext = static_cast<FQWGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		QW_EffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}