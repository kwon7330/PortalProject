


#include "AI/Portal_Turret.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"


APortal_Turret::APortal_Turret()
{
 	
	PrimaryActorTick.bCanEverTick = false;
//=====================================================================================================================

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Turret/turret_model.turret_model'"));
	if(TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//(X=0.000000,Y=0.000000,Z=-30.000000)
		GetMesh()->SetRelativeLocation(FVector(0,0,-30.0f));
		//(Pitch=0.000000,Yaw=-90.000000,Roll=0.000000)
		GetMesh()->SetRelativeRotation(FRotator(0,-90.f,0));
		GetMesh()->SetRelativeScale3D(FVector(0.01f));
	}

	AttackPoint = CreateDefaultSubobject<UArrowComponent>("AttackPoint");
	AttackPoint->SetupAttachment(RootComponent);
	//(X=25.000000,Y=0.000000,Z=3.600000)
	AttackPoint->SetRelativeLocation(FVector(25.0f,0.f,3.6f));
	
	
//=====================================================================================================================


}


void APortal_Turret::BeginPlay()
{
	Super::BeginPlay();
	
}


void APortal_Turret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APortal_Turret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

