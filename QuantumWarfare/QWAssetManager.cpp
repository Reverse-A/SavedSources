// @2023 All rights reversed by Reverse-Alpha Studios


#include "QWAssetManager.h"
#include "QWGameplayTags.h"
#include "AbilitySystemGlobals.h"


UQWAssetManager& UQWAssetManager::Get()
{
	check(GEngine);

	UQWAssetManager* QWAssetManager = Cast<UQWAssetManager>(GEngine->AssetManager);

	return *QWAssetManager;
}

void UQWAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FQWGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}
