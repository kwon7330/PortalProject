// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_Bullet.h"

// Sets default values
APortal_Bullet::APortal_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APortal_Bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

