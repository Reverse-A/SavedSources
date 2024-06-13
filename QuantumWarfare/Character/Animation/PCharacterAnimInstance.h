// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "QuantumWarfare/Character/Types/PlayerTypes.h"
#include "PCharacterAnimInstance.generated.h"

class ACharacterModule;
class APlayerCharacter;
UCLASS()
class QUANTUMWARFARE_API UPCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	// Function called when the animation instance is initialized
	virtual void NativeInitializeAnimation() override;
	// Function called to update the animation instance each frame
	virtual void NativeUpdateAnimation(float DeltaTime) override;


protected:
/*Classes*/
	UPROPERTY(BlueprintReadOnly, Category = Classes) 
	ACharacterModule* CharacterModule;


/*Variables*/

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) 
	float MovementSpeed;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"))
	float AimOffsetYaw;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"))
	float AimOffsetPitch;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true")) 
	float YawDelta;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"))
	bool bShouldMove = false;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"))
	bool bIsFalling = false;

	UPROPERTY(BlueprintReadOnly, meta = (PrivateAccess = "true"))
	EEquipmentState ABPEquipmentState;

public:
	void SetABPEquipmentState(EEquipmentState NewState)
	{ ABPEquipmentState = NewState; }

	EEquipmentState GetABPEquipmentState() const
	{ return ABPEquipmentState; }

	// SetPlayerCharacter
	void SetCharacterModule(ACharacterModule* InPC)
	{
		CharacterModule = InPC;
	}

private:
	void AimOffSet();

	void YawOffsetForLeaning(float ClampNegativeThreshold, float ClampPositiveThreshold);

	/* Aim Offset variables. */
private:
	// AimOffset Rotators variables
	FRotator StartingAimRotation;

	// AimYawDelta Rotators variables
	FRotator RotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;
};
