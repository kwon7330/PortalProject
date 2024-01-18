


#include "Object/Portal_Cube.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PortalProject/PortalProjectCharacter.h"


APortal_Cube::APortal_Cube()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp -> SetupAttachment(RootComponent);

	bReplicates = true;
	bAlwaysRelevant = true;
	// 위치값 복제
	SetReplicateMovement(true);
}


void APortal_Cube::BeginPlay()
{
	Super::BeginPlay();

	AActor* NewOwner = nullptr;
}


void APortal_Cube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal_Cube::Interact(APortalProjectCharacter* Character)
{
	UE_LOG(LogTemp,Warning,TEXT("fkdjfkdjsaflkjslfkja;sldkfj;alkjlk"))
	// 입력이 한번들어올떄마다  Cube Attach를 하고 Detach로 전환한다.
	// Attach Detach 전환 하는 변수가 필요하다. -> bCheck;
	if(bAttached == false)
	{
		UE_LOG(LogTemp,Warning,TEXT("dfdasffafdasf"));

		
		Attached(Character);
		
	}
	else
	{
		Detached(Character);
		
	}
}




void APortal_Cube::Attached(APortalProjectCharacter* Character)
{
	ServerRPC_Attached(Character);
}

void APortal_Cube::Detached(APortalProjectCharacter* Character)
{
	ServerRPC_Detached(Character);
}



//=========================================================================================================================

void APortal_Cube::MultiRPC_Detached_Implementation(APortalProjectCharacter* Character)
{
	MeshComp->SetSimulatePhysics(true);
	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	bAttached = false;
	Character->bHasCube = false;
}

void APortal_Cube::ServerRPC_Detached_Implementation(APortalProjectCharacter* Character)
{
	MultiRPC_Detached(Character);
}

void APortal_Cube::MultiRPC_Attached_Implementation(APortalProjectCharacter* Character)
{
	MeshComp->SetSimulatePhysics(false);
	MeshComp->AttachToComponent(Character->AttachComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	bAttached = true;
	Character->bHasCube = true;
}

void APortal_Cube::ServerRPC_Attached_Implementation(APortalProjectCharacter* Character)
{
	MultiRPC_Attached(Character);
}

void APortal_Cube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APortal_Cube,bAttached);
	
}






