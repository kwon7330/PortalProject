


#include "AI/TurretFSM.h"

#include "AI/Portal_Turret.h"
#include "Components/ArrowComponent.h"
#include "PortalProject/PortalProjectCharacter.h"
#include "Runtime/AIModule/Classes/AIController.h"


UTurretFSM::UTurretFSM()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UTurretFSM::BeginPlay()
{
	Super::BeginPlay();

	Target = Cast<APortalProjectCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Self = Cast<APortal_Turret>(GetOwner());
	AI =  Cast<AAIController>(Self->GetController());
	
	
}



void UTurretFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (State)
	{

	case ETurretState::Idle : TickIdle();
	case ETurretState::Attack: TickAttack();
	case ETurretState::Die : TickDie();
		
	}

	
}

void UTurretFSM::TickIdle()
{
	UE_LOG(LogTemp,Warning,TEXT("Idle@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"))
	Target = Cast<APortalProjectCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	float Distance = FVector::Dist(Self->GetActorLocation(),Target->GetActorLocation());
	if(Target != nullptr && Distance < AttackDist)
	{
		SetState(ETurretState::Attack);
	}
	
}

void UTurretFSM::TickAttack()
{
	UE_LOG(LogTemp,Warning,TEXT("Attack@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"))
	AI->SetFocus(Target,EAIFocusPriority::Gameplay);
	CurrentTime += GetWorld()->GetDeltaSeconds();
	if(CurrentTime>AttackTime)
	{
		Attack();
		SetState(ETurretState::Idle);
	}
	
}

void UTurretFSM::TickDie()
{
	
}

void UTurretFSM::Attack()
{
	UE_LOG(LogTemp,Warning,TEXT("Attacking@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"))
	FHitResult HitInfo;
	FVector StartPoint = Self->AttackPoint->GetComponentLocation();
	FVector EndPoint = StartPoint + Self->GetActorForwardVector() *  300;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Self);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo,StartPoint,EndPoint,ECC_Visibility,Params);
	DrawDebugLine(GetWorld(),StartPoint,EndPoint,FColor::Red,false,2,0,3);
	if(bHit == true)
	{
		// if(Target -> IsA<APortalProjectCharacter>())
		// {
		// 	// 체력을 감소시키고싶다.	
		// }
	}
	
}

void UTurretFSM::SetState(ETurretState Next)
{
	State = Next;
	CurrentTime = 0;
}

