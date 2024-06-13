// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flag.generated.h"

class ABaseCharacter;

class USphereComponent;
class USoundCue;

UENUM(BlueprintType)
enum class EFlagState : uint8
{
	EFS_Initial UMETA(DisplayName = "Initial State"),
	EFS_Equiped UMETA(DisplayName = "Equipped"),
	EFS_Dropped UMETA(DisplayName = "Dropped"),
	EFS_Scored  UMETA(DisplayName = "Reset"),

	EFS_MAX     UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class ETeamFlagColour : uint8
{
	ETFC_NoTeamFlag					UMETA(DisplayName = "NoTeam"),
	ETFC_VanguardSovereigntyFlag	UMETA(DisplayName = "VanguardSovereigntyTeam"),
	ETFC_GenesisPactFlag			UMETA(DisplayName = "GenesisPactTeam"),

	ETFC_MAX						UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class SYNTHETICA_API AFlag : public AActor
{
	GENERATED_BODY()
	
public:
	AFlag();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void FlagControlPoints(float DeltaSeconds);

private:
	UPROPERTY(VisibleAnywhere, Category = BPDeveloperProperties)
	UStaticMeshComponent* FlagStaticMesh;

	UPROPERTY(VisibleAnywhere, Category = BPDeveloperProperties)
	USphereComponent* FlagAreaSphere;

	UPROPERTY(VisibleAnywhere, Category = BPDeveloperProperties)
	USphereComponent* ScoreSphere;
#pragma region OverlapEvents
protected:
	bool bFlagOverlappedWithScorePoint = false;

	UFUNCTION()
	virtual void OnSphereOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);
#pragma endregion

#pragma region Flag-State

protected:
	UPROPERTY()
	ABaseCharacter* BaseCharacter;

	UPROPERTY(ReplicatedUsing = OnRep_FLagState, VisibleAnywhere, Category = "BPDeveloper Properties")
	EFlagState FlagState;

	UPROPERTY(Replicated)
	ETeamFlagColour TeamFlagColour = ETeamFlagColour::ETFC_NoTeamFlag;

	UFUNCTION()
	void OnRep_FlagState();

	void OnState_Flag();
	void OnState_Flag_Initial();
	void OnState_Flag_Equipped();
	void OnState_Flag_Dropped();
	void OnState_Flag_Scored();

public:
	void SetTeamFlagColour(ETeamFlagColour NewTeam) { TeamFlagColour = NewTeam; }
	FORCEINLINE ETeamFlagColour GetTeamFlagColour() const { return TeamFlagColour; }
#pragma endregion

#pragma region Other Functions
private:
	bool bFlagIsDropped = false;
	float CheckFlagStateTime;
public:
	void Drop_Flag();

#pragma region

#pragma region Transforms;
private:
	FVector		SpawnerLocation;
	FRotator	SpawnerRotation;
	FTransform	FlagInitialTransform;

public:
	void Set_FlagInitialTransform(FTransform Transform) { FlagInitialTransform = Transform; }
	void Set_FlagSpawnerLocation(FVector  SetLocation) { SpawnerLocation = SetLocation; }
	void Set_FlagSpawnerRotation(FRotator SetRotation) { SpawnerRotation = SetRotation; }

#pragma endregion


#pragma region DestroyFlag
private:
	FVector  FlagFirstLocation;
	FRotator FlagFirstRotation;

	FTimerHandle DestroyFlag_TimerHandle;

	UPROPERTY(EditAnywhere, Category = "BPDeveloperProperties")
	float FlagDestroyAfterDroppedTime = 7.0f;

	void TimerHandler_DestroyFlag();

#pragma endregion



#pragma region Sounds
	UPROPERTY(EditAnywhere, Category = BPDeveloperProperties)
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, Category = BPDeveloperProperties)
	USoundCue* ScoreSound;

	UPROPERTY(EditAnywhere, Category = BPDeveloperProperties)
	USoundCue* WrongScorePointSound;

public:
	void PlaySound_Equip_Flag();
	void PlaySound_ScoreFlag();
	void PlaySound_WrongScorePlace(ABaseCharacter * OwnerCharacter); 
#pragma endregion


#pragma region Getters&Setters
public:
	FORCEINLINE void Set_FlagState(EFlagState FlagState);
#pragma endregion
};
