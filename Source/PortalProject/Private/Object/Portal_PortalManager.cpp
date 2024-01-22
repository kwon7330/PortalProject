// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_PortalManager.h"
#include "PortalActor.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Object/Fizzler.h"
#include "Object/Portal_Tablet.h"

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

	TArray<AActor*> Fizzlers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFizzler::StaticClass(), Fizzlers);

	for (AActor* Fizzler : Fizzlers)
	{
		Cast<AFizzler>(Fizzler)->OnPlayerFizzled.AddUniqueDynamic(this, &APortal_PortalManager::OnPlayerFizzled);
	}
}

void APortal_PortalManager::SpawnPortal(APortalActor** OutPointer, APortalActor** OppositePointer, const EPortalType InType,
	const FTransform& PortalSpawnTransform, APawn* PortalOwner, APortal_Tablet* Tablet)
{
	if (*OutPointer)
	{
		(*OutPointer)->Destroy();
		*OutPointer = nullptr;
	}

	*OutPointer = GetWorld()->SpawnActorDeferred<APortalActor>(PortalClass, PortalSpawnTransform, PortalOwner);
	(*OutPointer)->Type = InType;
	UGameplayStatics::FinishSpawningActor(*OutPointer, PortalSpawnTransform);
	Tablet->PortalActor = (*OutPointer);

	if (*OppositePointer)
	{
		(*OutPointer)->LinkedPortal = *OppositePointer;
		(*OutPointer)->LinkWithOtherPortal();
		(*OppositePointer)->LinkedPortal = *OutPointer;
		(*OppositePointer)->LinkWithOtherPortal();
	}
}

void APortal_PortalManager::RequestPortal_Implementation(EPortalType Type, class APortal_Tablet* Tablet, APawn* PortalOwner)
{
	FTransform Transform = Tablet->SpawnPoint->GetComponentTransform();
	FTransform PortalSpawnTransform = FTransform(Transform.GetRotation(), Transform.GetTranslation(), FVector::OneVector);

	if (Tablet->PortalActor)
	{
		Tablet->PortalActor->Destroy();
		Tablet->PortalActor = nullptr;
	}
	
	switch (Type) {
	case EPortalType::Player1Blue:
		SpawnPortal(&BluePortal, &PurplePortal, Type, PortalSpawnTransform, PortalOwner, Tablet);
		break;
	case EPortalType::Player1Purple:
		SpawnPortal(&PurplePortal, &BluePortal, Type, PortalSpawnTransform, PortalOwner, Tablet);
		break;
	case EPortalType::Player2Orange:
		SpawnPortal(&OrangePortal, &RedPortal, Type, PortalSpawnTransform, PortalOwner, Tablet);
		break;
	case EPortalType::Player2Red:
		SpawnPortal(&RedPortal, &OrangePortal, Type, PortalSpawnTransform, PortalOwner, Tablet);
		break;
	}
}

void APortal_PortalManager::OnPlayerFizzled(EPlayerType Player)
{
	switch (Player) {
	case EPlayerType::PBody:
		if (BluePortal)
		{
			BluePortal->Destroy();
			BluePortal = nullptr;
		}
		if (PurplePortal)
		{
			PurplePortal->Destroy();
			PurplePortal = nullptr;
		}
		break;
	case EPlayerType::Atlas:
		if (OrangePortal)
		{
			OrangePortal->Destroy();
			OrangePortal = nullptr;
		}
		if (RedPortal)
		{
			RedPortal->Destroy();
			RedPortal = nullptr;
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

