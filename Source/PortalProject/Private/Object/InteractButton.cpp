


#include "Object/InteractButton.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Object/MovingFloor.h"
#include "Object/SphereBallFactory.h"
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
	bisOverlap= true;
	auto Player = Cast<APortalProjectCharacter>(OtherActor);
	if(Player != nullptr)
	{
		OnButtonStatusChanged.Broadcast(true);
		
		if(MovingFloor)
		{
			UE_LOG(LogTemp,Warning,TEXT("isMovingFloor"));
			ServerRPC_OnOverlap();
			
		}
		else if(BallFactory)
		{
			// 만약 월드 안에 모든 버튼이 눌린다면 공을 떨군다.
			BallFactory->CheckButtonOverlap();
		}
		//ServerRPC_OnOverlap();
	}
}

void AInteractButton::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bisOverlap = false;
	OnButtonStatusChanged.Broadcast(false);
	if(MovingFloor)
	{
		UE_LOG(LogTemp,Warning,TEXT("isMovingFloor"));
		ServerRPC_EndOverlap();
	}
}



void AInteractButton::ServerRPC_OnOverlap_Implementation()
{
	if(MovingFloor)
	{
		MovingFloor->MultiRPC_FloorAct();
	}
	else if(BallFactory)
	{
		// 만약 월드 안에 모든 버튼이 눌린다면 공을 떨군다.
		BallFactory->ServerRPC_CheckButtonOverlap();
	}
	
}

void AInteractButton::ServerRPC_EndOverlap_Implementation()
{
	MovingFloor->MultiRPC_ReturnAct();
}

void AInteractButton::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractButton, bisOverlap);
}