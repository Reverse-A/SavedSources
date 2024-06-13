// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "CharacterModule.h"
#include <QuantumWarfare/Character/Types/PlayerTypes.h>

#include "PlayerCharacter.generated.h"



/*Objects*/
class UArsenalComponent;
class UCameraComponent;
class USpringArmComponent;
class UPCharacterAnimInstance;
class UPlayerOverlay;

class AWeaponModule;



UCLASS()
class QUANTUMWARFARE_API APlayerCharacter : public ACharacterModule
{
	GENERATED_BODY()


#pragma region ParentClass[Functions] & Constructor
public:
    // Constructor
    APlayerCharacter();

    virtual void PreInitializeComponents() override;

    // Called when the player character is possessed by a controller
    virtual void PossessedBy(AController* NewController) override;

    // Called when the player state is replicated to clients
    virtual void OnRep_PlayerState() override;

    /*Combat Interface*/
    virtual int32 GetPlayerLevel() override;
 
#pragma endregion

#pragma region Components
private:
    // Spring Arm component for managing camera positioning
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArmComponent;

    // Camera component defining the player's viewpoint
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComponent;
public:
    UPCharacterAnimInstance* GetPlayerAnimationInstance() const { return CharacterAnimation; }
#pragma endregion

#pragma region AbilitySystem
private:
    virtual void InitAbilityActorInfo() override;

#pragma endregion

#pragma region WeaponSystem
public:
    void SetOverlappingWeapon(AWeaponModule* InputWeapon);

#pragma endregion

#pragma region Interaction
public:
    void Interaction();
#pragma endregion

protected:
    FName WeaponTipSocketName;
    virtual FVector GetCombatSocketLocation() override;


#pragma region Hit-Death Cond.
protected:
    virtual void Death() override;
#pragma endregion
};