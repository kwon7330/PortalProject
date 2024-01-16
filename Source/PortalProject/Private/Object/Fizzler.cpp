// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Fizzler.h"

#include "PortalProject/PortalProjectCharacter.h"

// Sets default values
AFizzler::AFizzler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AFizzler::PlaneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어인지 확인
	if (APortalProjectCharacter* PC = Cast<APortalProjectCharacter>(OtherActor))
	{
		// 플레이어라면 포탈 없애기
		
	}
}

// Called when the game starts or when spawned
void AFizzler::BeginPlay()
{
	Super::BeginPlay();
	Plane->OnComponentBeginOverlap.AddDynamic(this, &AFizzler::PlaneBeginOverlap);
}

