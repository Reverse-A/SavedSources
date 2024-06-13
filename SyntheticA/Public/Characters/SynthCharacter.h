// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "SynthCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API ASynthCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASynthCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMeshes")				
	USkeletalMeshComponent* Bag;

public:
	FORCEINLINE USkeletalMeshComponent* GetBagMesh()	const { return Bag; }
};
