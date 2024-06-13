// @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"


USTRUCT(BlueprintType)
struct FBoxInformation
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	FVector BoxExtent;

	FBoxInformation()
		: Location(FVector::ZeroVector)
		, Rotation(FRotator::ZeroRotator)
		, BoxExtent(FVector::ZeroVector)
	{

	}
};

USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()

	UPROPERTY()
	float Time;

	UPROPERTY()
	TMap<FName, FBoxInformation> HitBoxInfo;

	UPROPERTY()
	ABaseCharacter* Character;

	FFramePackage()
		: Time(0.0f)
	{

	}
};

USTRUCT(BlueprintType)
struct FServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool bHitConfirmed;

	UPROPERTY()
	bool bHeadShot;

	FServerSideRewindResult()
		: bHitConfirmed(false)
		, bHeadShot(false)
	{
	}

	FServerSideRewindResult(bool HitConfirmed, bool HeadShot)
		: bHitConfirmed(HitConfirmed)
		, bHeadShot(HeadShot)
	{

	}
};

USTRUCT(BlueprintType)
struct FShotgunServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()	TMap<ABaseCharacter*, uint32> HeadShots;
	UPROPERTY()	TMap<ABaseCharacter*, uint32> BodyShots;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SYNTHETICA_API ULagCompensationComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	ULagCompensationComponent();

private:
	friend class ABaseCharacter;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

#pragma region FramePackage
private:
	void SaveFramePackage				(FFramePackage& Package);
	void ShowFramePackage				(const FFramePackage& Package, const FColor& Color);

	void CacheBoxPositions				(ABaseCharacter* HitCharacter, FFramePackage& OutFramePackage);
	void MoveBoxes						(ABaseCharacter* HitCharacter, const FFramePackage& Package);
	void ResetHitBoxes					(ABaseCharacter* HitCharacter, const FFramePackage& Package);
	void EnableCharacterMeshCollision	(ABaseCharacter* HitCharacter, ECollisionEnabled::Type CollisionEnabled);
	void SaveFramePackage();

private:
	FFramePackage GetFrameToCheck		(ABaseCharacter* HitCharacter, float HitTime);

	FFramePackage InterpBetweenFrames	(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime);

#pragma region HitConfirmation
public:

	/*HitScan*/
	FServerSideRewindResult ConfirmHit
	(
		const FFramePackage& Package,
		ABaseCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize& HitLocation
	);

	/*Shotgun*/
	FShotgunServerSideRewindResult ShotgunConfirmHit
	(
		const TArray<FFramePackage>& FramePackages,
		const FVector_NetQuantize& TraceStart,
		const TArray<FVector_NetQuantize>& HitLocations
	);

	/*Projectile*/
	FServerSideRewindResult ProjectileConfirmHit
	(
		const FFramePackage& Package,
		ABaseCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize100& InitialVelocity,
		float HitTime
	);
#pragma endregion
#pragma region SSR
protected:
/*HitScan*/
	FServerSideRewindResult ServerSideRewind
	(
		class ABaseCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize& HitLocation,
		float HitTime
	);


/*Shotgun*/
	FShotgunServerSideRewindResult ShotgunServerSideRewind
	(
		const TArray<ABaseCharacter*>& HitCharacters, 
		const FVector_NetQuantize& TraceStart,
		const TArray<FVector_NetQuantize>& HitLocations,
		float HitTime
	);

/*Projectile*/
	FServerSideRewindResult ProjectileServerSideRewind
	(
		ABaseCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize100& InitialVelocity,
		float HitTime
	);

#pragma endregion


#pragma region Server
public:
	UFUNCTION(Server, Reliable)
	void Server_ScoreRequest
	(
		ABaseCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize& HitLocation,
		float HitTime
	);

	UFUNCTION(Server, Reliable)
	void Server_ShotgunScoreRequest
	(
		const TArray<ABaseCharacter*>& HitCharacters,
		const FVector_NetQuantize& TraceStart,
		const TArray<FVector_NetQuantize>& HitLocations,
		float HitTime
	);

	UFUNCTION(Server, Reliable)
	void Server_ProjectileScoreRequest
	(
		ABaseCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize100& InitialVelocity,
		float HitTime
	);

#pragma endregion

private:
	UPROPERTY()
	ABaseCharacter* Character;

	UPROPERTY()
	class APlayerControllerManager* ControllerManager;


#pragma region FrameHistory
private:
	TDoubleLinkedList<FFramePackage> FrameHistory;

	UPROPERTY(EditAnywhere)
	float MaxRecordTime = 4.0f;
#pragma endregion
#pragma endregion
};