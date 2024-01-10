


#include "Object/Portal_Cube.h"


APortal_Cube::APortal_Cube()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp -> SetupAttachment(RootComponent);
	
}


void APortal_Cube::BeginPlay()
{
	Super::BeginPlay();
	
}


void APortal_Cube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

