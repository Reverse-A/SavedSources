// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerStateModule.h"

#include "QuantumWarfare/Ability/AbilitySystem.h"
#include "QuantumWarfare/Ability/AttributeSystem.h"
#include <Net/UnrealNetwork.h>

APlayerStateModule::APlayerStateModule()
{
	// Create the Ability System Component and set it to replicate
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystem>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAttributeSystem>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f; 

	FString NewName = "LocalComputer-1";
	SetPlayerName(NewName);	

}

void APlayerStateModule::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerStateModule, Level);
}

void APlayerStateModule::OnRep_Level(int32 OldLevel)
{

}

//void APlayerStateModule::BeginPlay()
//{
//	Super::BeginPlay();
//
//
//	//// Get the owning player controller
//	//APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
//	//if (PlayerController && PlayerController->IsLocalController())
//	//{
//	//	// Set the player's display name
//	//	FString NewName = "LocalComputer-1";
//	//	SetPlayerName(NewName);
//	//}
//}

UAbilitySystemComponent* APlayerStateModule::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
