// @2023 "Age Of Reverse" Story. All rights reversed. // | Reverse - A Company.|

#pragma once

#include "CoreMinimal.h"
#include "ECameraPerspective.h"

#include "Components/ActorComponent.h"
#include "CameraConfiguration.generated.h"

class ABaseCharacter;
class APlayerHUD;
class APlayerControllerManager;

//class UCoreAnimProcessor;
//class UCombatComponent;
class UEnhancedMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNTHETICA_API UCameraConfiguration : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Constructors
	UCameraConfiguration();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Functions
public:	
	void SwitchCamera();
	void SwitchToFirstPersonCamera();
	void SwitchToThirdPersonCamera();
	void ResetCameraSwitchCooldown();

	void FirstPersonCamera();
	void ThirdPersonCamera();

	void GeneralCameraSystemFunctionOperation();


	void CrouchCameraInterpolation();
/*Classes*/
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

	UPROPERTY()
	UEnhancedMovementComponent* EnhancedMovementComponent;

/* CameraPerspective Enumerator */
private:
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	ECameraViewPerspective CameraViewPerspective;

public:
	FORCEINLINE ECameraViewPerspective GetCameraViewPerspective() const { return CameraViewPerspective; }

	UFUNCTION(BlueprintCallable, Category = Camera)
	void SetCameraViewPerspective(ECameraViewPerspective NewPerspective) {	CameraViewPerspective = NewPerspective;	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Camera)
	bool IsCameraViewPerspective(ECameraViewPerspective PerspectiveToCheck) const;

private:
	/*Camera Switch*/

	FTimerHandle CameraSwitchCooldownTimerHandle;

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraSwitchCooldownTime = 0.5f;

	bool bCameraSwitchCooldown = false;

};
