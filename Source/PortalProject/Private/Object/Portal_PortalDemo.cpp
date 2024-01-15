


#include "Object/Portal_PortalDemo.h"
#include "Materials/MaterialInterface.h"

APortal_PortalDemo::APortal_PortalDemo()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	SetRootComponent(MeshComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Portal/KDJ/Meshes/SM_Portal.SM_Portal'"));
	if(TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(0.5f));
		MeshComp->SetRelativeRotation(FRotator(0,90,0));
		
		Materials = MeshComp->GetMaterials();
		if(Materials.Num() > 0)
		{
			BluePortal = Materials[0];
		}
		if(Materials.Num() > 1)
		{
			PurplePortal = Materials[1];
		}
	}
	
}


void APortal_PortalDemo::BeginPlay()
{
	Super::BeginPlay();
	
}


void APortal_PortalDemo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}




