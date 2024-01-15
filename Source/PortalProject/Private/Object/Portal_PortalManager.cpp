// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_PortalManager.h"

// Sets default values
APortal_PortalManager::APortal_PortalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APortal_PortalManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal_PortalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

