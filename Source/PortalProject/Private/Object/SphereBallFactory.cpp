


#include "Object/SphereBallFactory.h"
#include "Components/ArrowComponent.h"
#include "Object/SphereBall.h"


ASphereBallFactory::ASphereBallFactory()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	SetRootComponent(MeshComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Resources/ItemDispenser/Dispenser_0.Dispenser_0'"));
	if(TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	ArrowComp = CreateDefaultSubobject<UArrowComponent>("ArrowComp");
	ArrowComp ->SetupAttachment(MeshComp);
	ArrowComp -> SetRelativeRotation(FRotator(-90,0,0));
}


void ASphereBallFactory::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASphereBallFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASphereBallFactory::FallingBall()
{
	ServerRPC_FallingBall();
}

void ASphereBallFactory::MultiRPC_FallingBall_Implementation()
{
	FTransform SpawnPoint = ArrowComp->GetComponentTransform();
	GetWorld()->SpawnActor<ASphereBall>(BallFactory,SpawnPoint);
}

void ASphereBallFactory::ServerRPC_FallingBall_Implementation()
{
	MultiRPC_FallingBall();
}

