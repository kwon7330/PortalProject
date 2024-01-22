// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalProject/PortalProject.h"
#include "Portal_PortalManager.generated.h"

class APortal_PortalDemo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPortalCreated, EPortalType, PortalType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPortalDestroyed, EPortalType, PortalType);

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

	FPortalCreated OnPortalCreated;
	FPortalDestroyed OnPortalDestroyed;
	
	void SpawnPortal(class APortalActor** OutPointer, APortalActor** OppositePointer, const EPortalType InType,
					 const FTransform& PortalSpawnTransform, APawn* PortalOwner, class APortal_Tablet* Tablet);

	UFUNCTION()
	void OnPlayerFizzled(EPlayerType Player);
	
	UPROPERTY(Replicated, EditAnywhere, Category="PortalManager|Portals")
	APortalActor* BluePortal;
	UPROPERTY(Replicated, EditAnywhere, Category="PortalManager|Portals")
	APortalActor* PurplePortal;
	UPROPERTY(Replicated, EditAnywhere, Category="PortalManager|Portals")
	APortalActor* RedPortal;
	UPROPERTY(Replicated, EditAnywhere, Category="PortalManager|Portals")
	APortalActor* OrangePortal;
	
	UPROPERTY(EditAnywhere, Category = "PortalManager|Setting")
	TSubclassOf<class APortalActor> PortalClass;

	UFUNCTION(Server, Reliable)
	void RequestPortal(EPortalType Type, class APortal_Tablet* Tablet, APawn* PortalOwner);

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastPortalCreated(EPortalType Type);

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastPortalDeleted(EPortalType Type);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;
	
	/*TMap<EPortalType, APortal_PortalDemo*> PortalMap;*/
	
};
