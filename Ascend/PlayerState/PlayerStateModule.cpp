// @2023 All rights reversed by Reverse-Alpha Studios


#include "PlayerStateModule.h"
#include "Ascend/Character/BaseCharacter.h"
APlayerStateModule::APlayerStateModule()
{

}



void APlayerStateModule::BeginPlay()
{
	Super::BeginPlay();

	BaseCharacter = Cast<ABaseCharacter>(GetPawn());

	//if (BaseCharacter)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter casting is successful"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("ABaseCharacter casting is not successful"));
	//}
}