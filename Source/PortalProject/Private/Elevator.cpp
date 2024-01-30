// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"

#include "FCTween.h"


// Sets default values
AElevator::AElevator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AElevator::TravelToServer()
{
	FCTween::ClearActiveTweens();
	GetWorld()->ServerTravel(TEXT("/Game/Portal/KHO/Maps/Room02_KHO"));
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

