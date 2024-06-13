// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Vehicle/GroundMobileVehicle.h"
#include "GroundVehiclePawn.generated.h"
#pragma region Pre-Classes&Components
class ABaseCharacter;

class UAudioComponent;
class UCameraComponent;
class USpringArmComponent;
class USoundCue;
class UInputMappingContext;
class UInputAction;
class UChaosWheeledVehicleMovementComponent;
struct FInputActionValue;
#pragma endregion
UCLASS()
class SYNTHETICA_API AGroundVehiclePawn : public AGroundMobileVehicle
{
	GENERATED_BODY()

#pragma region Constructor &DefaultFunctions
public:
	AGroundVehiclePawn();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
#pragma endregion
#pragma region Components
private:
	/** Cast pointer to the Chaos Vehicle movement component */
	TObjectPtr<UChaosWheeledVehicleMovementComponent> ChaosVehicleMovement;

	UPROPERTY()
	ABaseCharacter* BaseCharacter = nullptr;

	/*Getters*/
public:
	/** Returns the cast Chaos Vehicle Movement subobject */
	FORCEINLINE const TObjectPtr<UChaosWheeledVehicleMovementComponent>& GetChaosVehicleMovement() const {
		return ChaosVehicleMovement;
	}
#pragma endregion

#pragma region Overlapping
	UFUNCTION()
	void OnBoxCollisionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion
#pragma region Camera-Properties
protected:
	/** Spring Arm for the front camera */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent*	FrontSpringArm;

	/** Front Camera component */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent*		FrontCamera;

	/** Spring Arm for the back camera */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent*	BehindSpringArm;

	/** Back Camera component */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent*		BehindCamera;

/*Getters*/
public:
	/** Returns the ground vehicle front spring arm subobject */
	FORCEINLINE USpringArmComponent* GetFrontSpringArm() const { return FrontSpringArm; }

	/* Returns the ground vehicle front camera subobject*/
	FORCEINLINE UCameraComponent* GetFrontCamera() const { return FrontCamera; }

	/* Returns the ground vehicle behind spring arm subobject*/
	FORCEINLINE USpringArmComponent* GetBehindSpringArm() const { return BehindSpringArm; }

	/* Returns the ground vehicle behind camera subobject*/
	FORCEINLINE UCameraComponent* GetBehindCamera() const { return BehindCamera; }



#pragma endregion
#pragma region Input-Properties &Functions
private:
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* InputGroundVehicleMappingContext;

/*Input Variables*/
	/** Steering Action */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SteeringAction;

	/** Throttle Action */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ThrottleAction;

	/** Brake Action */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* BrakeAction;

	/** Handbrake Action */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* HandbrakeAction;

	/** Look Around Action */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAroundAction;

	/** Toggle Camera Action */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ToggleCameraAction;

	/** Reset Vehicle Action */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ResetVehicleAction;

	/*Handling camera zoom in/out input Action.*/
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* CameraZoomInOutAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ExitGroundVehicleInputAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* PlayRadioInputAction;

	/** Keeps track of which camera is active */
	bool bFrontCameraActive = false;

/*Input Functions*/
	/*Handles sterring input*/
	void Steering(const FInputActionValue& Value);

	/*Handles throttle input*/
	void Throttle(const FInputActionValue& Value);
	void Brake(const FInputActionValue& Value);
	void StartBrake(const FInputActionValue& Value);
	void StopBrake(const FInputActionValue& Value);
	void StartHandbrake(const FInputActionValue& Value);
	void StopHandbrake(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void ResetVehicle(const FInputActionValue& Value);
	void BrakeLights(bool bBraking);
	void ExitVehicle();
	void InputAction_PlayRadio();


	/*Camera*/
	void InputAction_ToggleCamera(const FInputActionValue& Value);
	void InputAxis_ZoomInOut(const FInputActionValue& Value);


	bool bInterpolateZoom = false;
	void SmoothZoomInOut(bool bSmooth);
	float NewArmLength;
public:
	FORCEINLINE UInputMappingContext* Get_GroundVehicleInputMappingContext() const { return InputGroundVehicleMappingContext;}
#pragma endregion


#pragma region CurrentSituations
private:
	bool bPlayerEnteredVehicle;

public:
	void SetPlayerEnteredVehicle(bool bInside) { bPlayerEnteredVehicle = bInside; }
	FORCEINLINE bool GetCharacterInVehicle() const { return bPlayerEnteredVehicle; }
#pragma endregion
#pragma region Sounds
private:

/*Radio*/
	bool bRadioStarted = false;

	UPROPERTY(EditAnywhere)
	USoundCue* RadioPlaylist;

	UPROPERTY()
	UAudioComponent* RadioAudioComponent;

	void PlayRadio();
#pragma endregion
};
