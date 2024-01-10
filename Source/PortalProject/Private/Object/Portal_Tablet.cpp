


#include "Object/Portal_Tablet.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Object/Portal_Bullet.h"
#include "Object/Portal_Cube.h"


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

\
void APortal_Tablet::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&APortal_Tablet::OnOverlapBegin);
}

// Called every frame
void APortal_Tablet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal_Tablet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Overlaped!!!!!!!!!!!!!!!!!!!!!!!!!!"));
	// 포탈 총알이 부딪히면
	auto Bullet = Cast<APortal_Bullet>(OtherActor);
	if(Bullet != nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("IsBullet!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
		// 테블릿의 중앙에 포탈을 생성하고싶다.
		SpawnPortal();
	}
}

void APortal_Tablet::SpawnPortal()
{
	UE_LOG(LogTemp,Warning,TEXT("SpawnPortal@@@@@@@@@@@@@@@"));
	FTransform SpawnPortalPoint = SpawnPoint->GetComponentTransform();
	GetWorld()->SpawnActor<APortal_Cube>(SpawnFactory,SpawnPortalPoint);
	
}

