// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DoorActor.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//(X=0.000000,Y=-60.000000,Z=45.000000)
	MeshComp->SetRelativeLocation(FVector(0,-60,-45));
	//(Pitch=90.000000,Yaw=0.000000,Roll=-0.000000)
	MeshComp->SetRelativeRotation(FRotator(90,0,0));
	//(X=0.500000,Y=0.500000,Z=1.000000)
	MeshComp->SetRelativeScale3D(FVector(0.5,0.5,1));
	
	SecondMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("SecondMeshComp");
	//(X=0.000000,Y=-60.000000,Z=45.000000)
	SecondMeshComp->SetRelativeLocation(FVector(0,60,-45));
	//(Pitch=90.000000,Yaw=0.000000,Roll=-0.000000)
	SecondMeshComp->SetRelativeRotation(FRotator(90,0,0));
	//(X=0.500000,Y=0.500000,Z=1.000000)
	SecondMeshComp->SetRelativeScale3D(FVector(0.5,0.5,1));

	bReplicates = true;
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorActor::DoorActivated()
{
	ServerRPC_DoorAct();	
}

void ADoorActor::DoorReturn()
{
	ServerRPC_DoorRet();
}



void ADoorActor::ServerRPC_DoorAct_Implementation()
{
	MultiRPC_DoorAct();
}

void ADoorActor::ServerRPC_DoorRet_Implementation()
{
	MultiRPC_DoorRet();
}

void ADoorActor::MultiRPC_DoorAct_Implementation()
{
	// float Time = GetWorld()->GetDeltaSeconds();
	// FRotator NewRot = FMath::Lerp(FixRot,MeshComp->GetComponentRotation(),Time*0.2);
	// MeshComp->SetRelativeRotation(NewRot);
	MeshComp->SetVisibility(false);
	
}

void ADoorActor::MultiRPC_DoorRet_Implementation()
{
	// float Time = GetWorld()->GetDeltaSeconds();
	// FRotator NewRot = FMath::Lerp(MeshComp->GetComponentRotation(),FixRot,Time*0.2);
	// MeshComp->SetRelativeRotation(NewRot);

	MeshComp->SetVisibility(true);
}



