


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
	// 기본 대기상태
	// 플레이어를 바라보지않는다.
	AI->ClearFocus(EAIFocusPriority::Gameplay);
	//UE_LOG(LogTemp,Warning,TEXT("Idle@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"))
	Target = Cast<APortalProjectCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	float Distance = FVector::Dist(Self->GetActorLocation(),Target->GetActorLocation());
	// 타겟을 찾아서 공격 가능거리보다 가까워지면
	if(Target != nullptr && Distance < AttackDist)
	{
		// 공격 상태로 전이
		SetState(ETurretState::Attack);
	}
	
}

void UTurretFSM::TickAttack()
{
	//UE_LOG(LogTemp,Warning,TEXT("Attack@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"))
	// 상대방을 바라보고
	AI->SetFocus(Target,EAIFocusPriority::Gameplay);
	CurrentTime += GetWorld()->GetDeltaSeconds();
	// 현재 시간이 공격시간을 넘어서면
	if(CurrentTime>AttackTime)
	{
		// 공격을 하고 대기상태로 돌아간다.
		Attack();
		SetState(ETurretState::Idle);
	}
	
}

void UTurretFSM::TickDie()
{
	
}

void UTurretFSM::Attack()
{
	//UE_LOG(LogTemp,Warning,TEXT("Attacking@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"))
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

