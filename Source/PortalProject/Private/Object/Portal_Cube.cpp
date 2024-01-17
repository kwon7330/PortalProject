


#include "Object/Portal_Cube.h"

#include "Kismet/GameplayStatics.h"
#include "PortalProject/PortalProjectCharacter.h"


APortal_Cube::APortal_Cube()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp -> SetupAttachment(RootComponent);
	
}


void APortal_Cube::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APortalProjectCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}


void APortal_Cube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal_Cube::Interact_Implementation()
{
	IInteractable::Interact_Implementation();

	// 입력이 한번들어올떄마다  Cube Attach를 하고 Detach로 전환한다.
	// Attach Detach 전환 하는 변수가 필요하다. -> bCheck;
	if(bAttached == false)
	{
		Attached();
	}
	else
	{
		Detached();
		
	}
}

void APortal_Cube::Attached()
{

	MeshComp->SetSimulatePhysics(false);
	MeshComp->AttachToComponent(Player->AttachComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	bAttached = true;
}

void APortal_Cube::Detached()
{
	MeshComp->SetSimulatePhysics(true);
	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	bAttached = false;
}








