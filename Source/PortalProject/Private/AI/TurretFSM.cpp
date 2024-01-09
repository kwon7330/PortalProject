


#include "AI/TurretFSM.h"

#include "AI/Portal_Turret.h"
#include "Components/ArrowComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"


UTurretFSM::UTurretFSM()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UTurretFSM::BeginPlay()
{
	Super::BeginPlay();

	Self = Cast<APortal_Turret>(GetOwner());
	Ai =  Cast<AAIController>(Self->GetController());
	
	
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
	Target = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(Target != nullptr)
	{
		SetState(ETurretState::Attack);
	}
	
}

void UTurretFSM::TickAttack()
{
	Ai->SetFocus(Target,EAIFocusPriority::Gameplay);
	CurrentTime += GetWorld()->GetDeltaSeconds();
	if(CurrentTime>AttackTime)
	{
		
	}
}

void UTurretFSM::TickDie()
{
	
}

void UTurretFSM::RaserAttack()
{
	// FVector StartPoint = Self->AttackPoint->GetComponentLocation()
	// FVector
	// Self->Get
}

void UTurretFSM::SetState(ETurretState Next)
{
	State = Next;
	CurrentTime = 0;
}

