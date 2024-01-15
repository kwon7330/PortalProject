// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_Bullet.h"

#include "Chaos/Deformable/ChaosDeformableCollisionsProxy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Object/Portal_PortalDemo.h"
#include "Object/Portal_PortalManager.h"
#include "Object/Portal_Tablet.h"
#include "Components/ArrowComponent.h"


APortal_Bullet::APortal_Bullet()
{
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp -> InitialSpeed = 2000;
	ProjectileMovementComp -> MaxSpeed = 2000;
}



void APortal_Bullet::BeginPlay()
{
	Super::BeginPlay();

	SphereComp -> OnComponentBeginOverlap.AddDynamic(this,&APortal_Bullet::OnOverlapBegin);
	PortalManager = Cast<APortal_PortalManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APortal_PortalManager::StaticClass()));

	checkf(PortalManager, TEXT("맵에 Portal Manager가 없다"));
	/*
	APortal_PortalDemo* Portal = *PortalManager->PortalMap.Find(Type);
	
	if (Portal != nullptr)
	{
		Portal->Destroy();
	}

	PortalManager->PortalMap.Add(Type, SpawnActor);
	*/
}
	



void APortal_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void APortal_Bullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Tablet = Cast<APortal_Tablet>(OtherActor);
	if(Tablet != nullptr)
	{
		switch (Type)
		{
		case EPortalType::Player1Blue:
			{
				if (PortalManager->BluePortal != nullptr)
				{
					PortalManager->BluePortal->Destroy();
				}
				// 스폰
				FTransform SpawnPortalPoint1 = Tablet->SpawnPoint->GetComponentTransform();
				PortalManager->BluePortal = GetWorld()->SpawnActor<APortal_PortalDemo>(Tablet->SpawnFactory,SpawnPortalPoint1);	
			}
			break;
		case EPortalType::Player1Purple:
			{
				if(PortalManager->PurplePortal != nullptr)
				{
					PortalManager->BluePortal->Destroy();
				}
				FTransform SpawnPortalPoint2 = Tablet->SpawnPoint->GetComponentTransform();
				PortalManager->BluePortal = GetWorld()->SpawnActor<APortal_PortalDemo>(Tablet->SpawnFactory,SpawnPortalPoint2);
			}
			break;
		
		case EPortalType::Player2Orange:
			// if(PortalManager != nullptr)
			// {
			// 	PortalManager->OrangePortal->Destroy();
			// }
			break;
		case EPortalType::Player2Red:
			break;
		}
	}
}















