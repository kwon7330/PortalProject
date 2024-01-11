


#include "Object/Portal_CloseBullet.h"

#include "NavigationSystemTypes.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


APortal_CloseBullet::APortal_CloseBullet()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SetRootComponent(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp -> SetupAttachment(SphereComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
	ProjectileMovementComp -> InitialSpeed = 2000;
	ProjectileMovementComp -> MaxSpeed = 2000;
}


void APortal_CloseBullet::BeginPlay()
{
	Super::BeginPlay();
	
}


void APortal_CloseBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

