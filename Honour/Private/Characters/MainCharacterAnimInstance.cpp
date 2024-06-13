// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacterAnimInstance.h"
#include "Characters/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<AMainCharacter>(TryGetPawnOwner());

	if (PlayerCharacter)
	{
		PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
	}
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacterMovement)
	{
		GroundSpeed			   = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);
		IsFalling			   = PlayerCharacterMovement->IsFalling();
		CharacterEquippedState = PlayerCharacter->GetCharacterEquippedState();
		ActionState			   = PlayerCharacter->GetActionState();
		DeathPose			   = PlayerCharacter->GetDeathPose();
	}
}
