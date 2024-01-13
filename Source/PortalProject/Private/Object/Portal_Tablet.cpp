


#include "Object/Portal_Tablet.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Object/Portal_Bullet.h"
#include "Object/Portal_CloseBullet.h"
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


void APortal_Tablet::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&APortal_Tablet::OnOverlapBegin);
}


void APortal_Tablet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal_Tablet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp,Warning,TEXT("Overlaped!!!!!!!!!!!!!!!!!!!!!!!!!!"));
	// 포탈 총알이 부딪히면 총알이 포탈을 여는 총알이면 여는 포탈을 생성한다.
	// 포탈이 생성 되었을 때 하나라도 존재하면 기존에 있던 포탈을 없에고 다시 포탈을 생성한다.
	auto Portal = Cast<APortal_Bullet>(OtherActor);
	
	if(Portal!= nullptr)
	{
		if(Portal->Type == EPortalType::Player1Blue)
		{
			
			UE_LOG(LogTemp,Warning,TEXT("BluePortal"));
			
			//SameColorPortal.Add(NewBluePortal);
		}
		else if(Portal->Type == EPortalType::Player1Purple)
		{
			//APortal_Bullet* NewPurplePortal;
			UE_LOG(LogTemp,Warning,TEXT("PurplePortal"));
			//SameColorPortal.Add(NewPurplePortal);
		}
		else if(Portal->Type == EPortalType::Player2Orange)
		{
			//APortal_Bullet* NewOrangePortal;
			UE_LOG(LogTemp,Warning,TEXT("OrangePortal"));
			//SameColorPortal.Add(NewOrangePortal);
		}
		else if(Portal->Type == EPortalType::Player2Red)
		{
			//APortal_Bullet* NewRedPortal;
			UE_LOG(LogTemp,Warning,TEXT("RedPortal"));
			//SameColorPortal.Add(NewRedPortal);

		}
	}
}

void APortal_Tablet::SpawnPortal()
{
	FTransform SpawnPortalPoint = SpawnPoint->GetComponentTransform();
	GetWorld()->SpawnActor<APortal_Cube>(SpawnFactory,SpawnPortalPoint);
}


