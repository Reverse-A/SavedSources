// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

UCLASS()
class QUANTUMWARFARE_API UTargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(	
		BlueprintCallable, 
		Category = "Ability|Tasks",
		meta = (
		DisplayName = "TargetDataUnderCursor",
		HidePin = "OwningAbility",
		DefaultToSelf = "OwningAbility",
		BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderCursor* CreateTargetDataUnderCursor(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	virtual void Activate() override;
	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
