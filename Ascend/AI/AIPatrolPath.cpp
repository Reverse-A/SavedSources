// @2023 All rights reversed by Reverse-Alpha Studios


#include "AIPatrolPath.h"
#include "Components/BillboardComponent.h"

#if TOGGLE_TEXT_RENDER
#include "Components/TextRenderComponent.h"
static FName NAME_TextRender(TEXT("Text Render Component"));
static FName NAME_VisualMesh(TEXT("Visual Mesh"));
#endif

AAIPatrolPath::AAIPatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;


	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));


#if TOGGLE_TEXT_RENDER
	BillboardComponent->SetupAttachment(RootComponent); 
	BillboardComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f)); 
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(NAME_VisualMesh);
	SetRootComponent(VisualMesh);
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	VisualMesh->SetHiddenInGame(true);

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(NAME_TextRender);
	TextRenderComponent->SetupAttachment(BillboardComponent); 
	TextRenderComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f)); 
	TextRenderComponent->SetText(FText::FromString("PatrolPoint"));
	TextRenderComponent->SetTextRenderColor(FColor::White);
	TextRenderComponent->SetVisibility(true, true);
#endif
}

FVector AAIPatrolPath::GetAIPatrolPoint(int const Index) const
{
	return AIPatrolPoints[Index];
}

int AAIPatrolPath::Num() const
{
	return AIPatrolPoints.Num();
}

