// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Portal_SmallButton.generated.h"

class APortal_Screen;
class UCapsuleComponent;

UCLASS()
class PORTALPROJECT_API APortal_SmallButton : public AActor ,public IInteractable
{
	GENERATED_BODY()
	
public:	
	
	APortal_SmallButton();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ButtonComp;

	UPROPERTY(EditAnywhere)
	APortal_Screen* Screen;

	

	void TimeOver();

public:
	virtual void Interact(APortalProjectCharacter* Character) override;
	
};
