// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Fizzler.h"

#include "Components/BoxComponent.h"
#include "Object/Portal_Bullet.h"
#include "PortalProject/PortalProjectCharacter.h"

// Sets default values
AFizzler::AFizzler()
	:
	RootComp(CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"))),
	Plane(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"))),
	Box(CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box Comp")))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(RootComp);
	Plane->SetupAttachment(RootComponent);
	Box->SetupAttachment(RootComponent);
}

void AFizzler::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어인지 확인
	if (APortalProjectCharacter* PC = Cast<APortalProjectCharacter>(OtherActor))
	{
		OnPlayerFizzled.Broadcast(PC->PlayerType);
	}

	if (APortal_Bullet* Bullet = Cast<APortal_Bullet>(OtherActor))
	{
		Bullet->Destroy();
		// TODO: add animation?
	}
}

// Called when the game starts or when spawned
void AFizzler::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &AFizzler::BoxBeginOverlap);
}

