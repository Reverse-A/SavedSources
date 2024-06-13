// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/Bird.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 1200.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    SceneComponent->SetupAttachment(GetRootComponent());

    Fuselage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fuselage"));
    Fuselage->SetupAttachment(SceneComponent);

    Glass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Glass"));
    Glass->SetupAttachment(Fuselage);

    RudderL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RudderL"));
    RudderL->AttachToComponent(Fuselage, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("rudderL"));
    RudderR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RudderR"));
    RudderR->AttachToComponent(Fuselage, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("rudderR"));

    ElevatorL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorL"));
    ElevatorL->AttachToComponent(Fuselage, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("elevatorL"));

    ElevatorR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorR"));
    ElevatorR->AttachToComponent(Fuselage, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("elevatorR"));

    AileronL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AileronL"));
    AileronL->AttachToComponent(Fuselage, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("aileronL"));

    AileronR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AileronR"));
    AileronR->AttachToComponent(Fuselage, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("aileronR"));

    flapsL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlapsL"));
    flapsL->AttachToComponent(Fuselage, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("flapsL"));

    flapsR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlapsR"));
    flapsR->AttachToComponent(Fuselage, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("flapsR"));
}
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
}
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

