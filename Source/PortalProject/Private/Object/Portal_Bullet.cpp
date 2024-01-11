// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_Bullet.h"

#include "Chaos/Deformable/ChaosDeformableCollisionsProxy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


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

	switch (Type)
	{
	case EPortalType::Player1Blue:
		break;
	case EPortalType::Player1Purple:
		break;
	case EPortalType::Player2Orange:
		break;
	case EPortalType::Player2Red:
		break;
	
	}
}


void APortal_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}







