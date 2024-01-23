


#include "Object/CircleDoor.h"

#include "CircleDoorAnim.h"


ACircleDoor::ACircleDoor()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComp");
	SetRootComponent(MeshComp);
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Door/door_-_from_portal_2_original.door_-_from_portal_2_original'"));
	if(TempMesh.Succeeded())
	{
		MeshComp->SetSkeletalMesh(TempMesh.Object);
	}
	bReplicates = true;
}


void ACircleDoor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ACircleDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACircleDoor::DoorOpen()
{
	UE_LOG(LogTemp,Warning,TEXT("DoorOpen"));
	ServerRPC_DoorOpen();
}


void ACircleDoor::DoorClose()
{
	UE_LOG(LogTemp,Warning,TEXT("DoorClose"));
	ServerRPC_DoorClose();
}

void ACircleDoor::MultiRPC_DoorClose_Implementation()
{
	MeshComp->PlayAnimation(CloseDoorAnim,true);
}

void ACircleDoor::ServerRPC_DoorClose_Implementation()
{
	MultiRPC_DoorClose();
}

void ACircleDoor::MultiRPC_DoorOpen_Implementation()
{
	MeshComp->PlayAnimation(OpenDoorAnim,true);
}

void ACircleDoor::ServerRPC_DoorOpen_Implementation()
{
	MultiRPC_DoorOpen();	
}

