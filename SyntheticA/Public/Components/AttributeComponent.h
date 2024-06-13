// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNTHETICA_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	friend class ABaseCharacter;
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


#pragma region Heal++
public:
	void Healing(float HealAmount, float HealingTime);
protected:
	void HealRampUp(float DeltaTime);
private:	
	UPROPERTY()
	class ABaseCharacter* BaseCharacter;

	bool bHealing = false;
	float HealingRate = 0.0f;
	float AmountToHeal = 0.0f;

/*Set&Get*/
public:
	FORCEINLINE bool IsHealing() const { return bHealing; }
	void SetHealing(bool Value) { bHealing = Value; }

#pragma endregion
#pragma region Shield++
public:
	void RegenerationShield(float ShieldAmount, float RefreshingTime);

protected:
	void ShieldRampUp(float DeltaTime);
private:
	bool bShieldRefreshing = false;
	
	float ShieldRefreshingRate = 0.0f;
	float ShieldRefreshingAmount = 0.0f;

public:
	FORCEINLINE bool IsShieldRefreshing() const { return bShieldRefreshing; }
	void SetShieldRefreshing(bool Value) { bShieldRefreshing = Value; }
#pragma endregion
#pragma region Speed++
public:
	void SpeedIncrement(float IncreaseWalkSpeed, float IncreaseCrouchedSpeed, float IncreaseSpeedTime);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpeedIncreament(float BaseWalkSpeed, float BaseCrouchedSpeed);

private:
	FTimerHandle SpeedIncreamentTimerHandler;

	void ResetSpeedIncreament();

	float InitialWalkSpeed;
	float InitialCrouchSpeed;

public:
	void SetInitialSpeed(float WalkSpeed, float CrouchSpeed);

#pragma endregion
#pragma region Jump++
public:
	void JumpIncrement(float IncreaseJumpVelocity, float IncreaseJumpTime);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_JumpIncrement(float IncreaseJumpVelocity);

private:
	FTimerHandle JumpIncreamentTimerHandler;

	void ResetJumpIncrement();

	float InitialJumpVelocity;

public:
	void SetInitialJumpVelocity(float JumpVelocity);

#pragma endregion
};
