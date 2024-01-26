// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyPortal.h"

#include "FCTween.h"
#include "Kismet/KismetMaterialLibrary.h"


// Sets default values
ADummyPortal::ADummyPortal()
	: RootComp(CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"))),
	  PortalEdgeMeshComp(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalEdge"))),
	  PortalPlane(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalPlane")))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(RootComp);
	PortalEdgeMeshComp->SetupAttachment(RootComp);
	PortalPlane->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ADummyPortal::BeginPlay()
{
	Super::BeginPlay();

	Color = *PortalColorMap.Find(Type);

	UMaterialInstanceDynamic* EdgeMeshMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(),
		PortalEdgeMeshComp->GetMaterial(0));
	EdgeMeshMat->SetVectorParameterValue(TEXT("PortalColor"), Color);
	PortalEdgeMeshComp->SetMaterial(0, EdgeMeshMat);

	UMaterialInstanceDynamic* PlaneMeshMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(),
		PortalPlane->GetMaterial(0));
	PlaneMeshMat->SetVectorParameterValue(TEXT("PortalColor"), Color);
	PortalPlane->SetMaterial(0, PlaneMeshMat);
	
	FCTween::Play(1, 0,
		[&](const float T)
		{
			SetActorScale3D(FVector::OneVector * T);
		}, 0.4)
	-> SetOnComplete([&] { Destroy(); });
}

// Called every frame
void ADummyPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

