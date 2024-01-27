// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IndicatorPanel.generated.h"

class AInteractButton;

UCLASS()
class PORTALPROJECT_API AIndicatorPanel : public AActor
{
	GENERATED_BODY()
	// #################################
	// ########### PROPERTIES ##########
	// #################################
public:
	UPROPERTY(BlueprintReadWrite)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* Checked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* NotChecked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AInteractButton* ConnectedButton;
	
	// #################################
	// ########### FUNCTIONS ###########
	// #################################
	
public:
	// Sets default values for this actor's properties
	AIndicatorPanel();

protected:
	UFUNCTION()
	void OnButtonChanged(bool bButtonStatus);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
