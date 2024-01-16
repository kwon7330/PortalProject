// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalProject/PortalProject.h"
#include "Portal_PortalManager.generated.h"

class APortal_PortalDemo;

UCLASS()
class PORTALPROJECT_API APortal_PortalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal_PortalManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere,Category="PortalManagerSettings")
	class APortalActor* BluePortal;
	UPROPERTY(EditAnywhere,Category="PortalManagerSettings")
	APortalActor* PurplePortal;
	UPROPERTY(EditAnywhere,Category="PortalManagerSettings")
	APortalActor* RedPortal;
	UPROPERTY(EditAnywhere,Category="PortalManagerSettings")
	APortalActor* OrangePortal;

	
	UPROPERTY(EditAnywhere, Category = "Portal")
	TSubclassOf<class APortalActor> PortalClass;
	
	void RequestPortal(EPortalType Type, const FTransform& Transform);

	/*TMap<EPortalType, APortal_PortalDemo*> PortalMap;*/
	
};
