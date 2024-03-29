// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_SmallButton.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Object/Portal_Screen.h"
#include "Object/SphereBallFactory.h"
#include "PortalProject/PortalProjectCharacter.h"


APortal_SmallButton::APortal_SmallButton()
{
 	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	SetRootComponent(MeshComp);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>StandMesh(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Switch/Switch_upstair_2_2.Switch_upstair_2_2'"));
	if(StandMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(StandMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(2.0f));
	}
	ButtonComp = CreateDefaultSubobject<UStaticMeshComponent>("ButtonComp");
	ButtonComp ->SetupAttachment(MeshComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh>ButtonMesh(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Switch/Switch_upstair_2_0.Switch_upstair_2_0'"));
	if(ButtonMesh.Succeeded())
	{
		ButtonComp->SetStaticMesh(ButtonMesh.Object);
	}

}


void APortal_SmallButton::BeginPlay()
{
	Super::BeginPlay();
	
}


void APortal_SmallButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}




void APortal_SmallButton::Interact(APortalProjectCharacter* Character)
{
	IInteractable::Interact(Character);
	if(Screen)
	{
		ServerRPC_ButtonInteract();
	}
	else if(Factory)
	{
		ServerRPC_SpButtonInteract();
	}
	else UE_LOG(LogTemp,Warning,TEXT("nullptr"));
	
	
	
}

void APortal_SmallButton::MultiRPC_ButtonInteract_Implementation()
{
	FVector SpawnSound = MeshComp->GetComponentLocation();
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(),SwitchActSound,SpawnSound);
	OnSmallButtonStatusChanged.Broadcast(true);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle,
		FTimerDelegate::CreateLambda(
			[&]
			{
				OnSmallButtonStatusChanged.Broadcast(false);
			}),
			5.0f,
			false);
}

void APortal_SmallButton::ServerRPC_SpButtonInteract_Implementation()
{
	Factory->FallingBall();
	MultiRPC_ButtonInteract();
}

void APortal_SmallButton::ServerRPC_ButtonInteract_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("CallMoveScreen"));
	Screen->ScreenMove();
	MultiRPC_ButtonInteract();

}







