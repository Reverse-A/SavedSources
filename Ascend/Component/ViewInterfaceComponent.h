// // @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ascend/HUD/PlayerHUD.h"
#include "ViewInterfaceComponent.generated.h"


class ABaseCharacter;
class UCombatSystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASCEND_API UViewInterfaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UViewInterfaceComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Components & Classes
protected:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

	UPROPERTY()
	APlayerController* OwnerPlayerController;
		
	UPROPERTY()
	APlayerHUD* PlayerHUD;

	UPROPERTY()
	UCombatSystemComponent* CombatSystem;

public:
	FORCEINLINE void SetBaseCharacter(ABaseCharacter* Insert) { BaseCharacter = Insert; }
#pragma endregion

#pragma region Crosshair
private:
	FCrosshairPackage CrosshairPackage;

	UPROPERTY(EditAnywhere, Category = DeveloperProperties)
	UTexture2D* CrosshairCenter;

	UPROPERTY(EditAnywhere, Category = DeveloperProperties)
	UTexture2D* CrosshairLeft;

	UPROPERTY(EditAnywhere, Category = DeveloperProperties)
	UTexture2D* CrosshairRight;

	UPROPERTY(EditAnywhere, Category = DeveloperProperties)
	UTexture2D* CrosshairTop;

	UPROPERTY(EditAnywhere, Category = DeveloperProperties)
	UTexture2D* CrosshairBottom;

private:
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor; 
	float CrosshairShootingFactor;

protected:
	void DrawCrosshairToViewPort(float DeltaSeconds);
#pragma endregion
};
