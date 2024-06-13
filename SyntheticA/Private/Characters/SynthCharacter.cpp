// @2023 All rights reversed by Reverse-Alpha Studios


#include "Characters/SynthCharacter.h"

ASynthCharacter::ASynthCharacter(const FObjectInitializer& ObjectInitializer) 
: ABaseCharacter(ObjectInitializer)
{
	Bag = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bag"));
	Bag->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Bag->SetupAttachment(GetMesh());

}
