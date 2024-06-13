// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileBullet.h"

/*Ground Vehicle*/
#include "Aeronautical/Aerodyne.h"
#include "Vehicle/GroundVehiclePawn.h"

#include "Characters/BaseCharacter.h"
#include "Components/LagCompensationComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerManager.h"

AProjectileBullet::AProjectileBullet()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->InitialSpeed	= ProjectileSpeed;
	ProjectileMovementComponent->MaxSpeed		= ProjectileSpeed;
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());

	if (OwnerCharacter)
	{
		APlayerControllerManager* OwnerController = Cast<APlayerControllerManager>(OwnerCharacter->Controller);

		if (OwnerController)
		{
			const float DamageToCause = Hit.BoneName.ToString() == FString("head") ? HeadShotDamage : Damage;

			const float DamageToVehicles = FMath::FRandRange(50.0f, 75.0f);
			if (OwnerController->HasAuthority() && !bUseServerSideRewind)
			{
				if (OtherActor && OtherActor->IsA(ABaseCharacter::StaticClass()))
				{
					UGameplayStatics::ApplyDamage
					(
						OtherActor,
						DamageToCause,
						OwnerController,
						this,
						UDamageType::StaticClass()
					);
				}
				else if (OtherActor && OtherActor->IsA(AAerodyne::StaticClass()))
				{
					UGameplayStatics::ApplyDamage
					(
						OtherActor,
						DamageToVehicles,
						OwnerController,
						this,
						UDamageType::StaticClass()
					);
				}
				else if (OtherActor && OtherActor->IsA(AGroundVehiclePawn::StaticClass()))
				{
					UGameplayStatics::ApplyDamage
					(
						OtherActor,
						DamageToVehicles,
						OwnerController,
						this,
						UDamageType::StaticClass()
					);
				}

				Super::OnHit(HitComponent, OtherActor, OtherComponent, NormalImpulse, Hit);	
				return;
			}

			ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(OtherActor);
			if (bUseServerSideRewind && OwnerCharacter->GetLagCompensationComponent() && OwnerCharacter->IsLocallyControlled() && HitCharacter)
			{
				OwnerCharacter->GetLagCompensationComponent()->Server_ProjectileScoreRequest
				(
					HitCharacter,
					TraceStart,
					InitialVelocity,
					OwnerController->GetServerTime() - OwnerController->GetSingleTripTime()
				);
			}
		}
	}
	Super::OnHit(HitComponent, OtherActor, OtherComponent, NormalImpulse, Hit);
}

void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
	
	//FPredictProjectilePathParams PathParams;
	//PathParams.bTraceWithChannel    = true;
	//PathParams.bTraceWithCollision  = true;
	//PathParams.DrawDebugTime		= 5.0f;
	//PathParams.DrawDebugType		= EDrawDebugTrace::ForDuration;
	//PathParams.LaunchVelocity		= GetActorForwardVector() * 7000.0f;
	//PathParams.MaxSimTime			= 4.0f;
	//PathParams.ProjectileRadius		= 5.0f;
	//PathParams.SimFrequency			= 30.0f;
	//PathParams.StartLocation			= GetActorLocation();
	//PathParams.TraceChannel			= ECollisionChannel::ECC_Visibility;
	//PathParams.ActorsToIgnore.Add(this);

	//FPredictProjectilePathResult PathResult;

	//UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);
}

#if WITH_EDITOR
void AProjectileBullet::PostEditChangeProperty(FPropertyChangedEvent& Event)
{
	Super::PostEditChangeProperty(Event);

	FName PropertyName = Event.Property != nullptr ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AProjectileBullet, ProjectileSpeed))
	{
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
			ProjectileMovementComponent->MaxSpeed	  = ProjectileSpeed;
		}
	}
}
#endif