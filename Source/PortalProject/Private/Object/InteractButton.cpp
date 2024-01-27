


#include "Object/InteractButton.h"

#include "Components/BoxComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Kismet/GameplayStatics.h"
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
	
	ButtonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ButtonMesh");
	ButtonMesh->SetupAttachment(Trigger);
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Button/button_-_from_portal_2_original.button_-_from_portal_2_original'"));
	if(TempMesh.Succeeded())
	{
		ButtonMesh->SetSkeletalMesh(TempMesh.Object);
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
		MultiRPC_OnOverLap();
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
	MultiRPC_EndOverLap();
	if(MovingFloor)
	{
		UE_LOG(LogTemp,Warning,TEXT("isMovingFloor"));
		ServerRPC_EndOverlap();
		
	}
	
}



void AInteractButton::MultiRPC_OnOverLap_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("PlayAnimation"));
	ButtonMesh->PlayAnimation(ButtonDownAnim,false);
	FVector SpawnSound = ButtonMesh->GetComponentLocation();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),ButtonDownSoundBase,SpawnSound);
}

void AInteractButton::MultiRPC_EndOverLap_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("PlayAnimation"));
	ButtonMesh->PlayAnimation(ButtonUpAnim,false);
	FVector SpawnSound = ButtonMesh->GetComponentLocation();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),ButtonUPSoundBase,SpawnSound);
}


void AInteractButton::ServerRPC_OnOverlap_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("ServerRPC_OnOverLap"));
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
	UE_LOG(LogTemp,Warning,TEXT("ServerRPC_EndOverLap"));
	MultiRPC_EndOverLap();
	MovingFloor->MultiRPC_ReturnAct();
}

void AInteractButton::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractButton, bisOverlap);
	
}