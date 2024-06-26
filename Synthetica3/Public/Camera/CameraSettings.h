// ©2023 Reverse - A Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUD/PlayerHUD.h"

#include "CameraSettings.generated.h"


class ABaseCharacter;
class APlayerHUD;
class APlayerControllerManager;

class UCoreAnimProcessor;
class UCombatComponent;
class UEnhancedMovementComponent;

UENUM(BlueprintType)
enum class ECameraViewPerspective : uint8
{
	ECVP_None	            UMETA(Hidden),                             // Hidden enumerator.
	ECVP_FirstPerson        UMETA(DisplayName = "FirstPersonCamera"),  // Represents the first-person camera view perspective.
	ECVP_ThirdPerson        UMETA(DisplayName = "ThirdPersonCamera"),  // Represents the third-person camera view perspective.

	ECVP_Custom_1           UMETA(DisplayName = "Reserved0"),          // Custom enumerator 0 (Reserved for future usage).
	ECVP_Custom_2           UMETA(DisplayName = "Reserved1"),          // Custom enumerator 1 (Reserved for future usage).
	ECVP_Custom_3           UMETA(DisplayName = "Reserved2"),          // Custom enumerator 2 (Reserved for future usage).
	ECVP_Custom_4           UMETA(DisplayName = "Reserved3"),          // Custom enumerator 3 (Reserved for future usage).
	ECVP_MAX                UMETA(Hidden),                             // Hidden enumerator used to indicate the maximum value.
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNTHETICA_API UCameraSettings : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCameraSettings();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	ABaseCharacter*					BaseCharacter;
	APlayerControllerManager*		PlayerControllerManager;
	APlayerHUD*						PlayerHUD;

	UCoreAnimProcessor*				CoreAnimProcessor;
	UCombatComponent*				CombatComponent;
	UEnhancedMovementComponent*		EnhancedMovement;
private:
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"), Replicated)
	ECameraViewPerspective CameraViewPerspective;

#pragma region | Camera Switch |
public:
	void SwitchCamera();
	void SwitchToFirstPersonCamera();
	void SwitchToThirdPersonCamera();

	UPROPERTY(EditAnywhere, Category = "Camera")
	UCurveFloat* CameraLocationLerpCurve;

	void PlaySwitchCameraAnimation(bool bChange);
	bool bShouldSwitchCameraAnimation;

	void CameraSystemOperation();

	void FirstPersonCamera();
	void ThirdPersonCamera();
	
	void HideCameraIfCharacterClose();

	void CharacterMeshesVisibility(bool Visibility);
	bool bMeshesVisible;

	UPROPERTY(EditAnywhere)
	float HideCameraThreshold = 200.f;

	void CrouchCameraInterpolation();
#pragma endregion

#pragma region | Camera || Tilt && Shake |
private:
	void CameraTick();
	void CameraTilt(float TargetXRoll);

	UFUNCTION(Client, Reliable)
	void Client_WallRunCameraShake();

public:
	UFUNCTION(Client, Reliable)
	void Client_ClimbCameraShake();
private:
	bool WallRun_CameraShakeCanPlayableAgain;
	float CameraTiltValue;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> Wallrun_CameraShake;
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> Climb_CameraShake;
	#pragma endregion

	/* 
	*  Other Properties 
	*/
private:
	
	/**
	* The sensitivity of mouse input for controlling the camera's look or view movement.
	* This property allows adjustment of how responsive the camera moves in response to mouse input.
	*/
	UPROPERTY(EditAnywhere, Category = Options)
	float MouseLookSensitivity;

#pragma region | HUD | Crosshair |
/*
* HUD && Crosshairs
*/
	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CircleTypeCrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CircleTypeCrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = Crosshairss)
	UTexture2D* CircleTypeCrosshairsRight;


	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	void SetHUDCrosshairs(float DeltaTime);
	#pragma endregion

#pragma region | Field Of View | 
private:
	//Field of view when not aiming; set to the camera's base FOV in Beginplay
	float DefaultFOV;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Options)
	float ZoomedFOV = 30.0f;

	UPROPERTY(EditAnywhere, Category = Options)
	float ZoomInterpSpeed = 20.0f;

	void InterpFOV(float DeltaTime);

#pragma endregion

#pragma region | Camera Switch |
private:
	/*Camera Switch*/
	void ResetCameraSwitchCooldown();

	FTimerHandle TimerHandle_CameraSwitchCooldown;

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraSwitchCooldown = 0.5f;

	bool bIsCameraSwitchOnCooldown = false;
#pragma endregion

#pragma region | Server |
public:
	// Function to set the camera view perspective
	UFUNCTION(BlueprintCallable, Category = Camera)
	void SetCameraViewPerspective(ECameraViewPerspective NewPerspective);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetCameraViewPerspective(ECameraViewPerspective NewPerspective);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetCameraViewPerspective(ECameraViewPerspective NewPerspective);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Camera)
	bool IsCameraViewPerspective(ECameraViewPerspective PerspectiveToCheck) const;

public:

	FORCEINLINE ECameraViewPerspective GetCameraViewPerspective()		const { return CameraViewPerspective; }
	FORCEINLINE float				   GetMouseLookSensitivityValue()	const { return MouseLookSensitivity;  }
	FORCEINLINE float				   GetCrosshairShootingFactor()		const { return CrosshairShootingFactor; }

	FORCEINLINE void				   SetCrosshairShootingFactor(float NewValue) { CrosshairShootingFactor = NewValue; }

#pragma endregion
};
