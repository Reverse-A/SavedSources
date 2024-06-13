// ©2023 Reverse - A Company.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomCameraComponent.generated.h"


UENUM(BlueprintType)
enum class ECameraViewPerspective : uint8
{
	ECVP					UMETA(Hidden),
	ECVP_FirstPerson		UMETA(DisplayName = "FirstPersonCamera"),
	ECVP_ThirdPerson		UMETA(DisplayName = "ThirdPersonCamera"),

	ECVP_Custom_1			UMETA(DisplayName = "CustomCameraView_1"),
	ECVP_Custom_2			UMETA(DisplayName = "CustomCameraView_2"),
	ECVP_Custom_3			UMETA(DisplayName = "CustomCameraView_3"),
	ECVP_Custom_4			UMETA(DisplayName = "CustomCameraView_4"),
	ECVP_MAX				UMETA(Hidden),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SYNTHETICA_API UCustomCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCustomCameraComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class ABaseCharacter* BaseCharacter;
	class UBaseCharacterMovementComponent* BaseCharacterMovementComponent;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	ECameraViewPerspective CameraViewPerspective;

public:
	void FirstPersonCamera();
	void ThirdPersonCamera();

private:
	UPROPERTY(EditAnywhere)
	float MouseLookSensitivity;

private:
	float ResetTurningTimer;

private:
	float AimOffSet_Yaw;

	bool CharacterIsTurningRight;
	bool CharacterIsTurningLeft;

	/*Get &Set &Question*/
public:
	// Function to set the camera view perspective
	UFUNCTION(BlueprintCallable, Category = Camera)
	void SetCameraViewPerspective(ECameraViewPerspective NewPerspective);

	// Function to get the current camera view perspective
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Camera)
	ECameraViewPerspective GetCameraViewPerspective() const { return CameraViewPerspective; }

	// Function to check if the current camera view perspective matches a specific perspective
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Camera)
	bool IsCameraViewPerspective(ECameraViewPerspective PerspectiveToCheck) const;


	FORCEINLINE float GetMouseLookSensitivity() const { return MouseLookSensitivity; }
};