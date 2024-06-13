// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorModule.generated.h"

class ABaseCharacter;
class AAIEntityModule;
class ABaseWeapon;
class ARangedWeapon;
class AMeleeWeapon;

UCLASS()
class ASCEND_API AActorModule : public AActor
{
	GENERATED_BODY()
	
public:	
	AActorModule();

protected:
	virtual void BeginPlay() override;

private:	
	// Static mesh component for the object
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

};
