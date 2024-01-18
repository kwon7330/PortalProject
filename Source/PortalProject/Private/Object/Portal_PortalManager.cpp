// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_PortalManager.h"
#include "PortalActor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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

	bReplicates = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void APortal_PortalManager::BeginPlay()
{
	Super::BeginPlay();

	check(PortalClass);
}

void APortal_PortalManager::RequestPortal_Implementation(EPortalType Type, const FTransform& Transform, APawn* PortalOwner)
{
	FTransform PortalSpawnTransform = FTransform(Transform.GetRotation(), Transform.GetTranslation(), FVector::OneVector);

	switch (Type) {
	case EPortalType::Player1Blue:
		if (BluePortal)
		{
			BluePortal->Destroy();
			BluePortal = nullptr;
		}

		BluePortal = GetWorld()->SpawnActorDeferred<APortalActor>(PortalClass, PortalSpawnTransform, PortalOwner);
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

		PurplePortal = GetWorld()->SpawnActorDeferred<APortalActor>(PortalClass, PortalSpawnTransform, PortalOwner);
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
		if (OrangePortal)
		{
			OrangePortal->Destroy();
			OrangePortal = nullptr;
		}

		OrangePortal = GetWorld()->SpawnActorDeferred<APortalActor>(PortalClass, PortalSpawnTransform, PortalOwner);
		OrangePortal->Type = Type;
		UGameplayStatics::FinishSpawningActor(OrangePortal, PortalSpawnTransform);

		if (RedPortal)
		{
			OrangePortal->LinkedPortal = RedPortal;
			OrangePortal->LinkWithOtherPortal();
			RedPortal->LinkedPortal = OrangePortal;
			RedPortal->LinkWithOtherPortal();
		}
		break;
	case EPortalType::Player2Red:
		if (RedPortal)
		{
			RedPortal->Destroy();
			RedPortal = nullptr;
		}

		RedPortal = GetWorld()->SpawnActorDeferred<APortalActor>(PortalClass, PortalSpawnTransform, PortalOwner);
		RedPortal->Type = Type;
		UGameplayStatics::FinishSpawningActor(RedPortal, PortalSpawnTransform);

		if (OrangePortal)
		{
			RedPortal->LinkedPortal = OrangePortal;
			RedPortal->LinkWithOtherPortal();
			OrangePortal->LinkedPortal = RedPortal;
			OrangePortal->LinkWithOtherPortal();
		}
		break;
	}
}

void APortal_PortalManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APortal_PortalManager, OrangePortal);
	DOREPLIFETIME(APortal_PortalManager, BluePortal);
	DOREPLIFETIME(APortal_PortalManager, RedPortal);
	DOREPLIFETIME(APortal_PortalManager, PurplePortal);
}

