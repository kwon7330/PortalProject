// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

class APortalProjectCharacter;

UCLASS()
class PORTALPROJECT_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComp;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SecondMeshComp;

	UPROPERTY(EditAnywhere)
	FRotator FixRot = FRotator(0.f);
	
	void DoorActivated();
	void DoorReturn();

	

	UFUNCTION(Server,Reliable)
	void ServerRPC_DoorAct();

	UFUNCTION(Server,Reliable)
	void ServerRPC_DoorRet();
	
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_DoorAct();

	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_DoorRet();
};
