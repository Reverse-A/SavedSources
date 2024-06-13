// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerCharacter.h"
#include "QuantumWarfare/UI/Widget/PlayerOverlay.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include <QuantumWarfare/Ability/AbilitySystem.h>
#include <QuantumWarfare/PlayerController/Player/PlayerControllerModule.h>
#include <QuantumWarfare/PlayerState/PlayerStateModule.h>
#include <QuantumWarfare/UI/HUD/PlayerHUD.h>

FName NAME_PlayerSpringArm(TEXT("SpringArm"));
FName NAME_PlayerCameraComp(TEXT("Camera"));

APlayerCharacter::APlayerCharacter()
{
    // Create a Spring Arm component
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(NAME_PlayerSpringArm);
    if (SpringArmComponent)
    {
        // Attach Spring Arm to the root component (assuming the root component is set elsewhere)
        SpringArmComponent->SetupAttachment(RootComponent);
        SpringArmComponent->TargetArmLength = 950.0f;
        SpringArmComponent->bInheritPitch   = false;
        SpringArmComponent->bInheritYaw     = false;
        SpringArmComponent->bInheritRoll    = false;
    }

    // Create a Camera component
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(NAME_PlayerCameraComp);
    if (CameraComponent)
    {
        // Attach Camera to Spring Arm
        CameraComponent->SetupAttachment(SpringArmComponent);
    }

    if (IsValid(GetCharacterMovement()))
    {
        GetCharacterMovement()->bOrientRotationToMovement = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
        GetCharacterMovement()->bConstrainToPlane   = true;
        GetCharacterMovement()->bSnapToPlaneAtStart = true;

        // Disable controller rotation axis
        bUseControllerRotationPitch = false;
        bUseControllerRotationYaw   = false;
        bUseControllerRotationRoll  = false;
    } 
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    InitAbilityActorInfo();
    AddCharacterAbilities();
}


void APlayerCharacter::PreInitializeComponents()
{
    Super::PreInitializeComponents();

    if (ArsenalComponent)
    {
        ArsenalComponent->SetPlayerCharacter(this);
    }
}

int32 APlayerCharacter::GetPlayerLevel()
{
    const APlayerStateModule* PlayerStateModule = GetPlayerState<APlayerStateModule>();
    check(PlayerStateModule);

    return PlayerStateModule->GetPlayerLevel();
}

void APlayerCharacter::InitAbilityActorInfo()
{
    APlayerStateModule* PlayerStateModule = GetPlayerState<APlayerStateModule>();
    check(PlayerStateModule);
    PlayerStateModule->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerStateModule, this);
    Cast<UAbilitySystem>(PlayerStateModule->GetAbilitySystemComponent())->AbilityActorInfoSet();
    AbilitySystemComponent = PlayerStateModule->GetAbilitySystemComponent();
    AttributeSet = PlayerStateModule->GetAttributeSet();

    if (APlayerControllerModule* PlayerControllerModule = Cast<APlayerControllerModule>(GetController()))
    {
        if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerControllerModule->GetHUD()))
        {
            PlayerHUD->InitOverlay(PlayerControllerModule, PlayerStateModule, AbilitySystemComponent, AttributeSet);
        }
    }
    InitializeDefaultAttributes();
}



void APlayerCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    InitAbilityActorInfo();
}



void APlayerCharacter::SetOverlappingWeapon(AWeaponModule* InputWeapon)
{
    if (ArsenalComponent != nullptr)
    {
        ArsenalComponent->SetOverlappingWeapon(InputWeapon);
    }
}

void APlayerCharacter::Interaction()
{
    if (ArsenalComponent != nullptr)
    {

    }   
}
FVector APlayerCharacter::GetCombatSocketLocation()
{
    Super::GetCombatSocketLocation();
    return ArsenalComponent->GetWeaponMuzzleFlashSocketLocation();
}
void APlayerCharacter::Death()
{
    Super::Death();
    ArsenalComponent->DropWeapon();
}
