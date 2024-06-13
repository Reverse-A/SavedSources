// ©2023 Reverse-A. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SYNTHETICA_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDAmmunation(int32 Ammo);
	void SetHUDCarriedAmmunation(int32 Ammo);
	void SetHUDGrenades(int32 Grenades);

protected:
	virtual void BeginPlay() override;

private:
	class ABaseHUD* BaseHUD;
	class ABaseCharacter* BaseCharacter;

	int32 HUDGrenades;
};
