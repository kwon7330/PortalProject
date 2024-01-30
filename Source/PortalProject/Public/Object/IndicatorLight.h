// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IndicatorLight.generated.h"

class APortal_SmallButton;
class AInteractButton;
class USplineComponent;

UCLASS()
class PORTALPROJECT_API AIndicatorLight : public AActor
{
	GENERATED_BODY()


// #################################
// ########### PROPERTIES ##########
// #################################
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineComponent* SplineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterial* DefaultMat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* DynamicMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AInteractButton* ConnectedButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APortal_SmallButton* ConnectedSmallButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor EnabledColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DisabledColor;

	
// #################################
// ########### FUNCTIONS ###########
// #################################
	
public:
	// Sets default values for this actor's properties
	AIndicatorLight();

protected:
	UFUNCTION()
	void ButtonStatusChanged(bool bButtonStatus);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(CallInEditor, BlueprintCallable)
	void SetColor(const FLinearColor& Color);
};
