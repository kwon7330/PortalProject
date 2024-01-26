// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DangerZone.generated.h"

class APortalProjectCharacter;
class UBoxComponent;

UCLASS()
class PORTALPROJECT_API ADangerZone : public AActor
{
	GENERATED_BODY()

public:
	
	// #################################
	// ########### FUNCTIONS ###########
	// #################################
	// Sets default values for this actor's properties
	ADangerZone();

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(Server, Reliable)
	void DamagePlayer(APortalProjectCharacter* Char);
	
	FTimerDelegate TimerDel;
	
	UPROPERTY()
	FTimerHandle P1Handle;

	UPROPERTY()
	FTimerHandle P2Handle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageDelay {1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FirstDamageTimeOffset {-0.5f};	
	
protected:
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// #################################
	// ########### PROPERTIES ##########
	// #################################
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;
	
};
