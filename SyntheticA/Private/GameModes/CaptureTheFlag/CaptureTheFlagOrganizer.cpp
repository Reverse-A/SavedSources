// @2023 All rights reversed by Reverse-Alpha Studios


#include "GameModes/CaptureTheFlag/CaptureTheFlagOrganizer.h"
#include "GameModes/CaptureTheFlag/Objects/Flag.h"

#include "Characters/BaseCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameModes/CaptureTheFlagGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "SyntheticA/SyntheticA.h"
#include "Net/UnrealNetwork.h"


ACaptureTheFlagOrganizer::ACaptureTheFlagOrganizer()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	FlagSpawner				 = CreateDefaultSubobject<UStaticMeshComponent>	(TEXT("FlagSpawner")); 
							   SetRootComponent(FlagSpawner);
	FlagSpawnerAreaSphere	 = CreateDefaultSubobject<USphereComponent>		(TEXT("FlagSpawnerAreaSphere"));
	FlagSpawnerAreaSphere	 ->SetupAttachment(FlagSpawner);

	WhiteTeamScorePoint		 = CreateDefaultSubobject<UStaticMeshComponent>	(TEXT("WhiteTeamScorePoint"));
	WhiteTeamScoreAreaSphere = CreateDefaultSubobject<USphereComponent>		(TEXT("WhiteTeamAreaSphere"));
	WhiteTeamScoreAreaSphere ->SetupAttachment(WhiteTeamScorePoint);

	BlackTeamScorePoint		 = CreateDefaultSubobject<UStaticMeshComponent>	(TEXT("BlackTeamScorePoint"));
	BlackTeamScoreAreaSphere = CreateDefaultSubobject<USphereComponent>		(TEXT("BlackTeamAreaSphere"));
	BlackTeamScoreAreaSphere ->SetupAttachment(BlackTeamScorePoint);


	float SphereRadius		 = 150.f;
	FlagSpawnerAreaSphere	->SetSphereRadius(SphereRadius);
	WhiteTeamScoreAreaSphere->SetSphereRadius(SphereRadius);
	BlackTeamScoreAreaSphere->SetSphereRadius(SphereRadius);

	WhiteTeamScoreAreaSphere->ShapeColor.Blue;
}

void ACaptureTheFlagOrganizer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


void ACaptureTheFlagOrganizer::BeginPlay()
{
	Super::BeginPlay();

	WhiteTeamScoreAreaSphere->SetCollisionObjectType(ECC_Flag);
	WhiteTeamScoreAreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WhiteTeamScoreAreaSphere->SetCollisionResponseToChannel(ECC_Flag, ECollisionResponse::ECR_Overlap);

	BlackTeamScoreAreaSphere->SetCollisionObjectType(ECC_Flag);
	BlackTeamScoreAreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BlackTeamScoreAreaSphere->SetCollisionResponseToChannel(ECC_Flag, ECollisionResponse::ECR_Overlap);

	WhiteTeamScoreAreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ACaptureTheFlagOrganizer::OnWhiteTeamSphereOverlap);
	BlackTeamScoreAreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ACaptureTheFlagOrganizer::OnBlackTeamSphereOverlap);


	BaseCharacter = BaseCharacter == nullptr ? Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()) : BaseCharacter;
	Spawn_FlagOnSpawner();

	bAttemptedWrongScorePlace = false;
}

void ACaptureTheFlagOrganizer::ExampleTeamValueWithForLoop()
{
	if (BaseCharacter)
	{
		ETeam Team		= BaseCharacter->GetTeam();
		int32 TeamValue = static_cast<int32>(Team);
		for (int8 EnumIndex = 0; EnumIndex < static_cast<int8>(ETS_MAX); ++EnumIndex)
		{
			ETeam CurrentTeam = static_cast<ETeam>(EnumIndex);
			FString TeamName = UEnum::GetValueAsString(CurrentTeam);
			UE_LOG(LogTemp, Warning, TEXT("Team enum value: %s"), *TeamName);
		}
	}
}

void ACaptureTheFlagOrganizer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ACaptureTheFlagOrganizer::Spawn_FlagOnSpawner()
{
	Server_InitializeFlagOnSpawner();
}

void ACaptureTheFlagOrganizer::Spawn_FlagAgain()
{
	Server_InitializeFlagOnSpawner();
}

void ACaptureTheFlagOrganizer::Server_InitializeFlagOnSpawner_Implementation()
{
	SpawnerLocation    = FlagSpawner->GetComponentLocation();
	SpawnerRotation    = FlagSpawner->GetComponentRotation();


	if (FlagComponent)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this;
			SpawnParameters.Instigator = GetInstigator();

			FlagClass = World->SpawnActor<AFlag>(FlagComponent, SpawnerLocation, SpawnerRotation, SpawnParameters);

			if (FlagClass)
			{
				FVector NewFlagLocation = FlagClass->GetActorLocation();
				NewFlagLocation.X += 50.0f;
				FlagClass->SetActorLocation(NewFlagLocation);
				FlagInitialTransform = FlagClass->GetTransform();
			}
		}
	}

}

void ACaptureTheFlagOrganizer::OnWhiteTeamSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FlagClass = Cast<AFlag>(OtherActor);
	if (FlagClass)
	{
		if (FlagClass->GetTeamFlagColour() == ETeamFlagColour::ETFC_VanguardSovereigntyFlag)
		{
			FlagClass->Set_FlagState(EFlagState::EFS_Scored);
			UWorld* World = GetWorld();
			ACaptureTheFlagGameMode* CTFGameMode = (ACaptureTheFlagGameMode*)World->GetAuthGameMode();
			if (CTFGameMode)
			{
				CTFGameMode->FlagCapturedScore(FlagClass);
				FlagClass->Destroy();
				Spawn_FlagAgain();
			}
		}

		if (FlagClass->GetTeamFlagColour() == ETeamFlagColour::ETFC_GenesisPactFlag)
		{
			if (bAttemptedWrongScorePlace == false)
			{
				if (BaseCharacter)
				{
					FlagClass->PlaySound_WrongScorePlace(BaseCharacter);
				}

				TimerHandler_AttempWrongScorePlaceStart();
			}
		}
	}

}

void ACaptureTheFlagOrganizer::OnBlackTeamSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FlagClass = Cast<AFlag>(OtherActor);

	if (FlagClass)
	{
		if (FlagClass->GetTeamFlagColour() == ETeamFlagColour::ETFC_VanguardSovereigntyFlag)
		{
			FlagClass->Set_FlagState(EFlagState::EFS_Scored);
			UWorld* World = GetWorld();
			ACaptureTheFlagGameMode* CTFGameMode = (ACaptureTheFlagGameMode*)World->GetAuthGameMode();
			if (CTFGameMode)
			{
				CTFGameMode->FlagCapturedScore(FlagClass);
				FlagClass->Destroy();
				Spawn_FlagAgain();
			}
		}

		if (FlagClass->GetTeamFlagColour() == ETeamFlagColour::ETFC_GenesisPactFlag)
		{
			if (bAttemptedWrongScorePlace == false)
			{
				if (BaseCharacter)
				{
					FlagClass->PlaySound_WrongScorePlace(BaseCharacter);
				}
				TimerHandler_AttempWrongScorePlaceStart();
			}
		}
	}

}

void ACaptureTheFlagOrganizer::TimerHandler_AttempWrongScorePlaceStart()
{
	float ResetDelay = 1.5f;
	GetWorldTimerManager().SetTimer
	(
		ResetTimer,
		this,
		&ACaptureTheFlagOrganizer::TimerHandler_AttempWrongScorePlaceReset,
		ResetDelay
	);

	bAttemptedWrongScorePlace = true;
}

void ACaptureTheFlagOrganizer::TimerHandler_AttempWrongScorePlaceReset()
{
	if (bAttemptedWrongScorePlace == true)
	{
		bAttemptedWrongScorePlace = false;
	}
}