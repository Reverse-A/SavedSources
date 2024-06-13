// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "PlayerWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FQWAttributeInfo;

class UAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FQWAttributeInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class QUANTUMWARFARE_API UAttributeMenuWidgetController : public UPlayerWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BindCallbacksDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
