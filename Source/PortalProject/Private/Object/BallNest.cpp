


#include "Object/BallNest.h"

#include "Components/BoxComponent.h"
#include "Object/CircleDoor.h"
#include "Object/SphereBall.h"


ABallNest::ABallNest()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	SetRootComponent(MeshComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Resources/BallButton/SM_BallButton.SM_BallButton'"));
	if(TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(13.0f));
	}
	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger -> SetupAttachment(MeshComp);
	Trigger -> SetRelativeScale3D(FVector(0.1f));
}


void ABallNest::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this,&ABallNest::OnOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this,&ABallNest::EndOverlap);
}


void ABallNest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABallNest::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Sphere = Cast<ASphereBall>(OtherActor);
	if(Sphere)
	{
		CircleDoor->DoorOpen();
	}
}

void ABallNest::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	auto Sphere = Cast<ASphereBall>(OtherActor);
	if(Sphere)
	{
		CircleDoor->DoorClose();
	}
}

