


#include "Object/SphereBallFactory.h"
#include "Components/ArrowComponent.h"
#include "Object/SphereBall.h"


ASphereBallFactory::ASphereBallFactory()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	ArrowComp = CreateDefaultSubobject<UArrowComponent>("ArrowComp");
	
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

