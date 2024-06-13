// @2023 All rights reversed by Reverse-Alpha Studios


#include "ActorModule.h"


static FName NAME_ActorModuleStaticMesh(TEXT("Static Mesh Component"));

AActorModule::AActorModule()
{
    // Prevent from unnecessary ticking
       PrimaryActorTick.bCanEverTick = false;

    // Set up static mesh component
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(NAME_ActorModuleStaticMesh);
    RootComponent       = StaticMeshComponent;
}

void AActorModule::BeginPlay()
{
	Super::BeginPlay();
}
