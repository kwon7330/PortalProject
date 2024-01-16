


#include "Object/Portal_Tablet.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Object/Portal_Bullet.h"
#include "Object/Portal_Cube.h"
#include "Object/Portal_PortalDemo.h"


APortal_Tablet::APortal_Tablet()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp -> SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint -> SetupAttachment(MeshComp);
	SpawnPoint -> SetRelativeScale3D(FVector(0.01,1,1));
}


void APortal_Tablet::BeginPlay()
{
	Super::BeginPlay();
}
