


#include "Object/MovingFloor.h"


AMovingFloor::AMovingFloor()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if(TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetVisibility(false);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComp->SetRelativeScale3D(FVector(30,20,0));
	}
	bReplicates = true;
}


void AMovingFloor::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMovingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovingFloor::ButtonInteract()
{
	IInteractable::ButtonInteract();
	UE_LOG(LogTemp,Warning,TEXT("FlorrAct111111111111111111"));
	MultiRPC_FloorAct();
}



void AMovingFloor::FloorAct()
{
	
	MultiRPC_FloorAct();
	
	
}

void AMovingFloor::Return()
{
	
	MultiRPC_ReturnAct();
	
}



void AMovingFloor::MultiRPC_FloorAct_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("FlorrAct2222222222222222222"));
	MeshComp->SetVisibility(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
}

void AMovingFloor::MultiRPC_ReturnAct_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("FloorReturn!!!!!!!!!!!!!!!!!!!"));
	MeshComp->SetVisibility(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



