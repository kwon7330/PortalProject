// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_Bullet.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PortalActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Object/Portal_PortalManager.h"
#include "Object/Portal_Tablet.h"
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
	ProjectileMovementComp -> InitialSpeed = 12000;
	ProjectileMovementComp -> MaxSpeed = 12000;

	TrailVfxComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail VFX Comp"));
	TrailVfxComp->SetupAttachment(MeshComp);
	
	MeshComp->SetNotifyRigidBodyCollision(true);
	
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicateMovement(true);

	SetLifeSpan(5.f);
}

void APortal_Bullet::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&APortal_Bullet::OnOverlapBegin);
	SphereComp->OnComponentHit.AddDynamic(this, &APortal_Bullet::OnMeshHit);
	PortalManager = Cast<APortal_PortalManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APortal_PortalManager::StaticClass()));

	checkf(PortalManager, TEXT("맵에 Portal Manager가 없다"));

	// 총알 색깔 변경
	UMaterialInstanceDynamic* BulletMeshMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), MeshComp->GetMaterial(0));
	BulletMeshMat->SetVectorParameterValue(TEXT("PortalColor"), *PortalColorMap.Find(Type));
	MeshComp->SetMaterial(0, BulletMeshMat);
	TrailVfxComp->SetVariableVec4(TEXT("TrailColor"), *PortalColorMap.Find(Type));
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
		PortalManager->RequestPortal(Type, Tablet, GetInstigator());
		this->Destroy();
	}
}

void APortal_Bullet::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UNiagaraComponent* Burst = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BurstVFX, GetActorLocation());
	Burst->SetVariableVec4(TEXT("Color"), *PortalColorMap.Find(Type));
	this->Destroy();
}

void APortal_Bullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APortal_Bullet, Type);
}















