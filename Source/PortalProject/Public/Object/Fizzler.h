// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fizzler.generated.h"

UCLASS()
class PORTALPROJECT_API AFizzler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFizzler();

	UFUNCTION()
	void PlaneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Plane;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
