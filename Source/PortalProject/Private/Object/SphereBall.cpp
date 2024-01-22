


#include "Object/SphereBall.h"

#include "Chaos/Deformable/ChaosDeformableCollisionsProxy.h"
#include "Net/UnrealNetwork.h"
#include "PortalProject/PortalProjectCharacter.h"


ASphereBall::ASphereBall()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	SphereMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("SphereMeshComp");
	SphereMeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Resources/EdglessCube/portal_2_edgeless_safety_cube.portal_2_edgeless_safety_cube'"));
	if(TempMesh.Succeeded())
	{
		SphereMeshComp->SetStaticMesh(TempMesh.Object);
		SphereMeshComp->SetRelativeScale3D(FVector(0.02f));
	}
	bReplicates = true;
	SetReplicateMovement(true);
}



void ASphereBall::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASphereBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASphereBall::Interact(APortalProjectCharacter* Character)
{
	IInteractable::Interact(Character);
	UE_LOG(LogTemp,Warning,TEXT("Interact!!!!!!!!!!!!"))
	if(bAttached == false)
	{
		UE_LOG(LogTemp,Warning,TEXT("PickUP!!!!!!!!!!!!!!!!!!"));

		
		SpAttached(Character);
		
	}
	else
	{
		SpDetached(Character);
		
	}
}

void ASphereBall::SpAttached(APortalProjectCharacter* Character)
{
	ServerRPC_SpAttached(Character);
}

void ASphereBall::SpDetached(APortalProjectCharacter* Character)
{
	ServerRPC_SpDetached(Character);
}

void ASphereBall::ServerRPC_SpDetached_Implementation(APortalProjectCharacter* Character)
{
	MultiRPC_SpDetached(Character);
}

void ASphereBall::ServerRPC_SpAttached_Implementation(APortalProjectCharacter* Character)
{
	MultiRPC_SpAttached(Character);
}

void ASphereBall::MultiRPC_SpDetached_Implementation(APortalProjectCharacter* Character)
{
	SphereMeshComp->SetSimulatePhysics(true);
	SphereMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	bAttached = false;
	Character->bHasCube = false;
}

void ASphereBall::MultiRPC_SpAttached_Implementation(APortalProjectCharacter* Character)
{
	SphereMeshComp->SetSimulatePhysics(false);
	SphereMeshComp->AttachToComponent(Character->AttachComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	bAttached = true;
	Character->bHasCube = true;
}

void ASphereBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASphereBall,bAttached);
	
}
