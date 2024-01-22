


#include "Object/CircleDoor.h"


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
	
}


void ACircleDoor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ACircleDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

