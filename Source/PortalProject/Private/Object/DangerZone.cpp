// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DangerZone.h"

#include "Components/BoxComponent.h"
#include "PortalProject/PortalProjectCharacter.h"


// Sets default values
ADangerZone::ADangerZone()
	:
	BoxComponent(CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp")))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(BoxComponent);
}

void ADangerZone::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APortalProjectCharacter* Char = Cast<APortalProjectCharacter>(OtherActor))
	{
		
	}
}

// Called when the game starts or when spawned
void ADangerZone::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADangerZone::OnBoxOverlap);
}
