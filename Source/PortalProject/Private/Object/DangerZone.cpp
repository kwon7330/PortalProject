// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/DangerZone.h"

#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PortalProject/PortalProjectCharacter.h"


// Sets default values
ADangerZone::ADangerZone()
	:
	BoxComponent(CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp")))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(BoxComponent);
	
	bAlwaysRelevant = true;
}

void ADangerZone::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APortalProjectCharacter* Char = Cast<APortalProjectCharacter>(OtherActor))
	{
		Char->GetCharacterMovement()->SetMovementMode(MOVE_Swimming);
		DamagePlayer(Char);
		switch (Char->PlayerType)
		{
		case EPlayerType::PBody:
			TimerDel.BindUFunction(this, TEXT("DamagePlayer"), Char);
			GetWorld()->GetTimerManager().SetTimer(
				P1Handle, TimerDel, DamageDelay, true, FirstDamageTimeOffset);
			break;
		case EPlayerType::Atlas:
			TimerDel.BindUFunction(this, TEXT("DamagePlayer"), Char);
			GetWorld()->GetTimerManager().SetTimer(
				P2Handle, TimerDel, DamageDelay, true, FirstDamageTimeOffset);
			break;
		}
	}
}

void ADangerZone::DamagePlayer_Implementation(APortalProjectCharacter* Char)
{
	Char->SetHP((Char->GetHP() - 1.f));
}

void ADangerZone::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APortalProjectCharacter* Char = Cast<APortalProjectCharacter>(OtherActor))
	{
		Char->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		switch (Char->PlayerType)
		{
		case EPlayerType::PBody:
			if (GetWorld()->GetTimerManager().TimerExists(P1Handle))
			{
				GetWorld()->GetTimerManager().ClearTimer(P1Handle);
			}
			break;
		case EPlayerType::Atlas:
			if (GetWorld()->GetTimerManager().TimerExists(P2Handle))
			{
				GetWorld()->GetTimerManager().ClearTimer(P2Handle);
			}
			break;
		}
	}
}

// Called when the game starts or when spawned
void ADangerZone::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADangerZone::OnBoxOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADangerZone::OnBoxEndOverlap);

	if (!HasNetOwner())
	{
		SetOwner(GetWorld()->GetFirstPlayerController());
	}
}
