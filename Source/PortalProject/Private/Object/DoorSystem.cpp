

#include "Object/DoorSystem.h"
#include "Components/BoxComponent.h"
#include "Object/Portal_Screen.h"
#include "PortalProject/PortalProjectCharacter.h"


ADoorSystem::ADoorSystem()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	SetRootComponent(TriggerBox);
	TriggerBox->SetBoxExtent(FVector(50.f,150.f,32.f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


void ADoorSystem::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this,&ADoorSystem::OnOverlap);
}


void ADoorSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorSystem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = Cast<APortalProjectCharacter>(OtherActor);
	if(Player != nullptr)
	{
		//플레이어가 오버랩 됬을때 문이 열리는 시스템을 만든다.
	}


	
}

