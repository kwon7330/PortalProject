// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fizzler.generated.h"

class UBoxComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerFizzled, EPlayerType, Player);

UCLASS()
class PORTALPROJECT_API AFizzler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFizzler();

	UFUNCTION()
	void BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FPlayerFizzled OnPlayerFizzled;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Plane;

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* PlaneMat;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
