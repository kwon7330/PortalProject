// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_Bullet.h"

#include "PortalActor.h"
#include "Chaos/Deformable/ChaosDeformableCollisionsProxy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Object/Portal_PortalDemo.h"
#include "Object/Portal_PortalManager.h"
#include "Object/Portal_Tablet.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Net/UnrealNetwork.h"


APortal_Bullet::APortal_Bullet()
{
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp -> InitialSpeed = 4000;
	ProjectileMovementComp -> MaxSpeed = 4000;

	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicateMovement(true);

	SetLifeSpan(5.f);
}



void APortal_Bullet::BeginPlay()
{
	Super::BeginPlay();

	SphereComp -> OnComponentBeginOverlap.AddDynamic(this,&APortal_Bullet::OnOverlapBegin);
	PortalManager = Cast<APortal_PortalManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APortal_PortalManager::StaticClass()));

	checkf(PortalManager, TEXT("맵에 Portal Manager가 없다"));


	// 총알 색깔 변경
	UMaterialInstanceDynamic* BulletMeshMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), MeshComp->GetMaterial(0));
	BulletMeshMat->SetVectorParameterValue(TEXT("PortalColor"), *PortalColorMap.Find(Type));
	MeshComp->SetMaterial(0, BulletMeshMat);
	
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
	UE_LOG(LogTemp, Warning, TEXT("Bullet Overlapped: %s"), *OtherActor->GetActorNameOrLabel());

	APortal_Tablet* Tablet = Cast<APortal_Tablet>(OtherActor);
	if (Tablet != nullptr)
	{
		PortalManager->RequestPortal(Type, Tablet->SpawnPoint->GetComponentTransform(), GetInstigator());
		this->Destroy();
	}
}

void APortal_Bullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APortal_Bullet, Type);
}















