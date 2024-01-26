// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/IndicatorPanel.h"

#include "Object/InteractButton.h"


// Sets default values
AIndicatorPanel::AIndicatorPanel()
	:
	RootComp(CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"))),
	StaticMeshComponent(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp")))
{
	SetRootComponent(RootComp);
	StaticMeshComponent->SetupAttachment(RootComp);
}

void AIndicatorPanel::OnButtonChanged(bool bButtonStatus)
{
	if (bButtonStatus)
	{
		StaticMeshComponent->SetMaterial(0, Checked);
	}
	else
	{
		StaticMeshComponent->SetMaterial(0, NotChecked);
	}
}

// Called when the game starts or when spawned
void AIndicatorPanel::BeginPlay()
{
	Super::BeginPlay();

	OnButtonChanged(false);
	if (ConnectedButton)
	{
		ConnectedButton->OnButtonStatusChanged.AddDynamic(this, &AIndicatorPanel::OnButtonChanged);
	}
	
}
