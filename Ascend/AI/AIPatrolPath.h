// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIPatrolPath.generated.h"

class UBillboardComponent;
class UTextRenderComponent;

#define TOGGLE_TEXT_RENDER 0


UCLASS()
class ASCEND_API AAIPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	AAIPatrolPath();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (MakeEditWidget="true", AllowPrivateAccess = "true"))
	TArray<FVector> AIPatrolPoints;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(VisibleAnywhere)
	UBillboardComponent* BillboardComponent;

#if TOGGLE_TEXT_RENDER
	UTextRenderComponent* TextRenderComponent;
#endif
public:
	FVector GetAIPatrolPoint(int const Index) const;
	int Num() const;
};
