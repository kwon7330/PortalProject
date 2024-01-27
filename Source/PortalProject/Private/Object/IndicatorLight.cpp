// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/IndicatorLight.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Object/InteractButton.h"


// Sets default values
AIndicatorLight::AIndicatorLight()
	:
	RootComp(CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"))),
	InstancedStaticMeshComponent(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Mesh"))),
	SplineComponent(CreateDefaultSubobject<USplineComponent>(TEXT("Spline Comp")))
{
	SetRootComponent(RootComp);
	InstancedStaticMeshComponent->SetupAttachment(RootComp);
	SplineComponent->SetupAttachment(RootComp);
}

void AIndicatorLight::ButtonStatusChanged(bool bButtonStatus)
{
	if (bButtonStatus == true)
	{
		SetColor(EnabledColor);
	}
	else
	{
		SetColor(DisabledColor);
	}
}

// Called when the game starts or when spawned
void AIndicatorLight::BeginPlay()
{
	Super::BeginPlay();

	DynamicMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), DefaultMat);
	InstancedStaticMeshComponent->SetMaterial(0, DynamicMat);

	if (ConnectedButton)
	{
		ConnectedButton->OnButtonStatusChanged.AddDynamic(this, &AIndicatorLight::ButtonStatusChanged);
	}
}

void AIndicatorLight::SetColor(const FLinearColor& Color)
{
	check(DynamicMat)
	DynamicMat->SetVectorParameterValue(FName("Color"), Color);
}
