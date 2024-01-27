


#include "Object/Portal_Screen.h"

#include "Components/BoxComponent.h"


APortal_Screen::APortal_Screen()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComp");
	SetRootComponent(MeshComp);
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/ArmInterior/arm_interior_192_-_from_portal_2_original.arm_interior_192_-_from_portal_2_original'"));
	if(TempMesh.Succeeded())
	{
		MeshComp->SetSkeletalMesh(TempMesh.Object);
	}

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionProfileName(FName("BlockAllDynamic"));
	
	bReplicates = true;
}


void APortal_Screen::BeginPlay()
{
	Super::BeginPlay();
	
}


void APortal_Screen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal_Screen::ScreenMove()
{
	ServerRPC_ScreenMove();
}

void APortal_Screen::ResetScreen()
{
	MultiRPC_ResetScreen();
}

void APortal_Screen::ServerRPC_ScreenMove_Implementation()
{
	MultiRPC_ScreenMove();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APortal_Screen::ResetScreen, 3.0f, false);
}

void APortal_Screen::MultiRPC_ResetScreen_Implementation()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Reset!!!!!!!!!!!!!!!!!!!!"));
	// float Time = GetWorld()->GetDeltaSeconds();
	// FRotator ResetRot = FMath::Lerp(ActivatedRot, RotBase, Time * 0.02f);
	// MeshComp->SetRelativeRotation(ResetRot);
	MeshComp->SetVisibility(true);
	//MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APortal_Screen::MultiRPC_ScreenMove_Implementation()
{
	
	// FRotator NewRot = FRotator(90, 90, -180);
	// float Time = GetWorld()->GetDeltaSeconds();
	// ActivatedRot = FMath::Lerp(RotBase, NewRot, Time * 0.02f);
	// MeshComp->SetRelativeRotation(ActivatedRot);

	MeshComp->SetVisibility(false);
	//MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



