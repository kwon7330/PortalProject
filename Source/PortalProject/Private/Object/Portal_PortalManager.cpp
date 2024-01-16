// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_PortalManager.h"
#include "PortalActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal_PortalManager::APortal_PortalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<APortalActor> PortalBPFinder {
		TEXT("Blueprint'/Game/Portal/KHO/Blueprints/Portal_BP.Portal_BP_C'")
	};

	if (PortalBPFinder.Succeeded())
	{
		PortalClass = PortalBPFinder.Class;
	}
	
}

// Called when the game starts or when spawned
void APortal_PortalManager::BeginPlay()
{
	Super::BeginPlay();

	check(PortalClass);
}

// Called every frame
void APortal_PortalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal_PortalManager::RequestPortal(EPortalType Type, const FTransform& Transform)
{
	FTransform PortalSpawnTransform = FTransform(Transform.GetRotation(), Transform.GetTranslation(), FVector::OneVector);

	switch (Type) {
	case EPortalType::Player1Blue:
		if (BluePortal)
		{
			BluePortal->Destroy();
			BluePortal = nullptr;
		}

		BluePortal = GetWorld()->SpawnActorDeferred<APortalActor>(PortalClass, PortalSpawnTransform);
		BluePortal->Type = Type;
		UGameplayStatics::FinishSpawningActor(BluePortal, PortalSpawnTransform);

		if (PurplePortal)
		{
			BluePortal->LinkedPortal = PurplePortal;
			BluePortal->LinkWithOtherPortal();
			PurplePortal->LinkedPortal = BluePortal;
			PurplePortal->LinkWithOtherPortal();
		}

		break;
	case EPortalType::Player1Purple:
		if (PurplePortal)
		{
			PurplePortal->Destroy();
			PurplePortal = nullptr;
		}

		PurplePortal = GetWorld()->SpawnActorDeferred<APortalActor>(PortalClass, PortalSpawnTransform);
		PurplePortal->Type = Type;
		UGameplayStatics::FinishSpawningActor(PurplePortal, PortalSpawnTransform);

		if (BluePortal)
		{
			PurplePortal->LinkedPortal = BluePortal;
			PurplePortal->LinkWithOtherPortal();
			BluePortal->LinkedPortal = PurplePortal;
			BluePortal->LinkWithOtherPortal();
		}

		break;
	case EPortalType::Player2Orange:
		break;
	case EPortalType::Player2Red:
		break;
	}
}

