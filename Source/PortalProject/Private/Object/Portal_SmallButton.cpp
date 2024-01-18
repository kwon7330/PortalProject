// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_SmallButton.h"

#include "Components/CapsuleComponent.h"
#include "Object/Portal_Screen.h"
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

void APortal_SmallButton::TimeOver()
{
	Screen->SetActorEnableCollision(true);
}

void APortal_SmallButton::Interact(APortalProjectCharacter* Character)
{
	IInteractable::Interact(Character);
	UE_LOG(LogTemp,Warning,TEXT("PushButton"));
	auto Wall = Cast<APortal_Screen>(Screen);
	if(Wall != nullptr)
	{
		FTimerHandle TimerHandle;
		
		GetWorldTimerManager().SetTimer(TimerHandle,this,&APortal_SmallButton::TimeOver,3.0f,false);	
		Wall->SetActorEnableCollision(false);
	}
}





