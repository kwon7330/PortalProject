// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_Screen.h"

// Sets default values
APortal_Screen::APortal_Screen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APortal_Screen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal_Screen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

