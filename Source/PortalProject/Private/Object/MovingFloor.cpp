


#include "Object/MovingFloor.h"


AMovingFloor::AMovingFloor()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	FTransform CurrentTransform = MeshComp->GetComponentTransform();
	FVector NewPivotLoc = FVector(0,0,200);
	CurrentTransform.SetLocation(NewPivotLoc);
	MeshComp->SetWorldTransform(CurrentTransform);
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
	FloorAct();
}



void AMovingFloor::FloorAct()
{
	
	MultiRPC_FloorAct();
	
}

void AMovingFloor::Return()
{
	
	MultiRPC_ReturnAct();
}

void AMovingFloor::MultiRPC_ReturnAct_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("FloorReturn!!!!!!!!!!!!!!!!!!!"));
	MeshComp->SetVisibility(true);
}

void AMovingFloor::MultiRPC_FloorAct_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("FlorrAct2222222222222222222"));
	MeshComp->SetVisibility(false);
	
}



