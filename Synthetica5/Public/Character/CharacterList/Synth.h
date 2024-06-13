// ©2023 Reverse - A Company.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Synth.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API ASynth : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ASynth(const FObjectInitializer& ObjectInitializer);


/*------------------------------------------------------------------------------------------------------------------------*/
//-------------------------//-------------------------/*Synethica*///------------------------//---------------------------*/
/*-----------------------------------------*[Character's Apperance Components]*-------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*---------These member variables represent the various meshes for the character's appearance and equipment.--------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Jacket;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Backpack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	USkeletalMeshComponent* Helmet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	UStaticMeshComponent*	AssaultRifle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	UStaticMeshComponent*	KatanaSheath;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")	UStaticMeshComponent*	Katana;
};
