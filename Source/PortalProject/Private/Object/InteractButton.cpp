


#include "Object/InteractButton.h"

#include "Components/BoxComponent.h"
#include "Object/MovingFloor.h"
#include "PortalProject/PortalProjectCharacter.h"


AInteractButton::AInteractButton()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	SetRootComponent(Trigger);
	Trigger->SetBoxExtent(FVector(50,50,32));
	
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>("ButtonMesh");
	ButtonMesh->SetupAttachment(Trigger);
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Button/SM_Button.SM_Button'"));
	if(TempMesh.Succeeded())
	{
		ButtonMesh->SetStaticMesh(TempMesh.Object);
	}

	bReplicates = true;
}


void AInteractButton::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this,&AInteractButton::OnOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this,&AInteractButton::EndOverlap);
	
}


void AInteractButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractButton::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = Cast<APortalProjectCharacter>(OtherActor);
	if(Player != nullptr)
	{
		if(MovingFloor)
		{
			UE_LOG(LogTemp,Warning,TEXT("isMovingFloor"));
			ServerRPC_OnOverlap();
			
		}
	}
}

void AInteractButton::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(MovingFloor)
	{
		UE_LOG(LogTemp,Warning,TEXT("isMovingFloor"));
		ServerRPC_EndOverlap();
	}
}

void AInteractButton::ServerRPC_EndOverlap_Implementation()
{
	MovingFloor->MultiRPC_ReturnAct();
}

void AInteractButton::ServerRPC_OnOverlap_Implementation()
{
	MovingFloor->ButtonInteract();
}

