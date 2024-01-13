// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal_SmallButton.generated.h"

class UCapsuleComponent;

UCLASS()
class PORTALPROJECT_API APortal_SmallButton : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APortal_SmallButton();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleComp;

	//플레이어가 버튼을 눌렀을 때
	UPROPERTY()
	bool PushedButton = false;
};
