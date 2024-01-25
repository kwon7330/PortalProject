


#include "Object/SphereBallFactory.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Object/InteractButton.h"
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

	bReplicates = true;
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

void ASphereBallFactory::CheckButtonOverlap()
{
	ServerRPC_CheckButtonOverlap();
}


void ASphereBallFactory::ServerRPC_CheckButtonOverlap_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("CheckButtonOverLap"));
	UWorld* World = GetWorld();
	TArray<AActor*> Buttons;
	UGameplayStatics::GetAllActorsOfClass(World, AInteractButton::StaticClass(), Buttons);

	bool AllButtonsOverlap = true;

	for (AActor* Actor : Buttons)
	{
		AInteractButton* InteractButton = Cast<AInteractButton>(Actor);
		if (InteractButton)
		{
			// 현재 InteractButton의 오버랩 상태 확인
			if (!InteractButton->bisOverlap)
			{
				// 하나라도 오버랩 상태가 아니라면 전체 오버랩이 아님
				AllButtonsOverlap = false;
				break;
			}
		}
	}
	
	if (AllButtonsOverlap)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Buttons overlapping"));
		FTransform SpawnPoint = ArrowComp->GetComponentTransform();
		SpawnPoint.SetScale3D(FVector(1,1,1));
		GetWorld()->SpawnActor<ASphereBall>(BallFactory,SpawnPoint);
		// 공이 스폰되면 더이상 스폰안되도록 만들어야한다.
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not all Buttons overlapping"));
	}
}

void ASphereBallFactory::ServerRPC_FallingBall_Implementation()
{
	MultiRPC_FallingBall();
}



void ASphereBallFactory::MultiRPC_FallingBall_Implementation()
{
	FTransform SpawnPoint = ArrowComp->GetComponentTransform();
	GetWorld()->SpawnActor<ASphereBall>(BallFactory,SpawnPoint);
}

