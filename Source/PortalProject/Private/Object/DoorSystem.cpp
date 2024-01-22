

#include "Object/DoorSystem.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Object/DoorActor.h"
#include "Object/Portal_Screen.h"
#include "PortalProject/PortalProjectCharacter.h"


ADoorSystem::ADoorSystem()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	SetRootComponent(TriggerBox);
	TriggerBox->SetBoxExtent(FVector(50.f,150.f,32.f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	bReplicates = true;
}


void ADoorSystem::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this,&ADoorSystem::OnOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this,&ADoorSystem::EndOverlap);
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
		CheckBox.AddUnique(Player);
		if(Door && CheckBox.Num() == 2)
		{
			//플레이어가 오버랩 됬을때 문이 열리는 시스템을 만든다.
			ServerRPC_DoorOpen();
		}
		
	}


	
}

void ADoorSystem::EndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Player = Cast<APortalProjectCharacter>(OtherActor);
	
		CheckBox.Remove(Player);

		if(Door)
		{
			ServerRPC_DoorClose();
		}	
	
}



void ADoorSystem::ServerRPC_DoorOpen_Implementation()
{
	Door->DoorActivated();
}

void ADoorSystem::ServerRPC_DoorClose_Implementation()
{
	Door->DoorReturn();
}


void ADoorSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoorSystem,CheckBox);
	
}