


#include "Object/Portal_Screen.h"


APortal_Screen::APortal_Screen()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComp");
	SetRootComponent(MeshComp);
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/ArmInterior/arm_interior_192_-_from_portal_2_original.arm_interior_192_-_from_portal_2_original'"));
	if(TempMesh.Succeeded())
	{
		MeshComp->SetSkeletalMesh(TempMesh.Object);
		MeshComp->SetCollisionProfileName("BlockAllDynamic");
	}
}


void APortal_Screen::BeginPlay()
{
	Super::BeginPlay();
	
}


void APortal_Screen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

