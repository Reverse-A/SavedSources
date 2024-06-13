// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CaptureTheFlagOrganizer.generated.h"
class ABaseCharacter;
class AFlag;

class UStaticMesh;
class USphereComponent;

UCLASS()
class SYNTHETICA_API ACaptureTheFlagOrganizer : public AActor
{
	GENERATED_BODY()
	
public:	
	ACaptureTheFlagOrganizer();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	void ExampleTeamValueWithForLoop();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditAnywhere, Category = BPDeveloperProps)	
	UStaticMeshComponent* WhiteTeamScorePoint;
	UPROPERTY(EditAnywhere, Category = BPDeveloperProps)
	UStaticMeshComponent* BlackTeamScorePoint;
	UPROPERTY(EditAnywhere, Category = BPDeveloperProps)
	UStaticMeshComponent* FlagSpawner;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* FlagSpawnerAreaSphere;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* WhiteTeamScoreAreaSphere;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* BlackTeamScoreAreaSphere;

	FTransform FlagInitialTransform;

	FVector  SpawnerLocation;
	FRotator SpawnerRotation;

/*Other-Classes*/
	UPROPERTY()	ABaseCharacter* BaseCharacter;

/*Tsubclass*/
	UPROPERTY(EditAnywhere, Category = BPDeveloperProps)
	TSubclassOf<AFlag> FlagComponent;

	AFlag* FlagClass;

private:
	void Spawn_FlagOnSpawner();
	void Spawn_FlagAgain();

	UFUNCTION(Server, Reliable)
	void Server_InitializeFlagOnSpawner();

	bool bAttemptedWrongScorePlace;


	FTimerHandle ResetTimer;
	void TimerHandler_AttempWrongScorePlaceStart();
	void TimerHandler_AttempWrongScorePlaceReset();

	UFUNCTION()
	void OnWhiteTeamSphereOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, 
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnBlackTeamSphereOverlap
	(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);
};
