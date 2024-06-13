// ©2023 Reverse - A Company.All rights reserved.


#include "Character/CharacterList/Synth.h"

ASynth::ASynth(const FObjectInitializer& ObjectInitializer)
: ABaseCharacter(ObjectInitializer)
{

	Jacket			= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Jacket"));
	Helmet			= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet"));
	Backpack		= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Backpack"));

	AssaultRifle	= CreateDefaultSubobject<UStaticMeshComponent>	(TEXT("Assault Rifle"));
	KatanaSheath	= CreateDefaultSubobject<UStaticMeshComponent>	(TEXT("Katana Sheath"));
	Katana			= CreateDefaultSubobject<UStaticMeshComponent>	(TEXT("Katana"));



	Jacket			->SetupAttachment(Torso);
	Helmet			->SetupAttachment(Torso);
	Backpack		->SetupAttachment(Torso);

	AssaultRifle	->SetupAttachment(Jacket);
	KatanaSheath	->SetupAttachment(Legs);
	Katana			->SetupAttachment(KatanaSheath);
}
