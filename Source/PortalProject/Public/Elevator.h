﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

UCLASS()
class PORTALPROJECT_API AElevator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElevator();

	UFUNCTION(BlueprintCallable)
	void TravelToServer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
