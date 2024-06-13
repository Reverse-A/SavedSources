#pragma once

#include "GameplayEffectTypes.h"
#include "AbilitySystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FQWGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	// returns the actual strcut used for serialization, subclasses must override this!
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FQWGameplayEffectContext* Duplicate() const
	{
		FQWGameplayEffectContext* NewContext = new FQWGameplayEffectContext();
		*NewContext = *this;

		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	// Custom serialization, subclasses must override this
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);


protected:
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsBlockedHit = false;

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit);
	void SetIsBlockedHit(bool bInIsBlockedHit); 
};

template<>
struct TStructOpsTypeTraits<FQWGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FQWGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};