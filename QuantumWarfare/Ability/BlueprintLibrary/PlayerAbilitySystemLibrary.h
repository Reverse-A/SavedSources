// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <QuantumWarfare/Ability/Data/CharacterClassInformation.h>
#include "PlayerAbilitySystemLibrary.generated.h"

class UAttributeMenuWidgetController;
class UPlayerOverlayWidgetController;

UCLASS()
class QUANTUMWARFARE_API UPlayerAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, Category = "PlayerAbilitySystemLibrary|WidgetController")
	static UPlayerOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "PlayerAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "PlayerAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes
	(
		const UObject* WorldContextObject, 
		ECharacterClass CharacterClass, 
		float Level,
		UAbilitySystemComponent* AbilitySystemComponent
	);

	UFUNCTION(BlueprintCallable, Category = "PlayerAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject,	UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "PlayerAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInformation* GetCharacterClassInformation(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "PlayerAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "PlayerAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "PlayerAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "PlayerAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

};
