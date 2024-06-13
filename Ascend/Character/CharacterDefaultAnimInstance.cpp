// // @2023 All rights reversed by Reverse-Alpha Studios


#include "CharacterDefaultAnimInstance.h"
#include "BaseCharacter.h"
#include "Ascend/Component/ArsenalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterDefaultAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	if (TryGetPawnOwner())
	{
		BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	}
}

void UCharacterDefaultAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(TryGetPawnOwner()) : BaseCharacter;

	if (BaseCharacter != nullptr)
	{
		bIsFalling		= BaseCharacter->GetCharacterMovement()->IsFalling();
		Velocity		= BaseCharacter->GetVelocity();
		CharacterSpeed	= BaseCharacter->GetVelocity().Size();

		//FRotator Rotation = BaseCharacter->GetActorRotation();
		//Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		//FVector PlaneProjectDirection = FVector::VectorPlaneProject(Velocity, Rotation.Vector()).GetSafeNormal();
		//RotateBonePitch = FMath::Clamp(BaseCharacter->GetBaseAimRotation().Pitch,-45.0f, 45.0f);

		CharacterForwardSpeed = BaseCharacter->GetForwardSpeed();
		CharacterRightSpeed   = BaseCharacter->GetRightSpeed();
		RotateBonePitch		  = BaseCharacter->GetRotateBonePitch();
		RotateBoneYaw		  = BaseCharacter->GetRotateBoneYaw();

		LeanRight			  = BaseCharacter->GetLeanRight();
	}
}

void UCharacterDefaultAnimInstance::FinishReloading()
{
	if (BaseCharacter)
	{
		BaseCharacter->SetReloading(false);
	}
}