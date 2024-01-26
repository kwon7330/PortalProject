// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Fizzler.h"

#include "FCTween.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Object/Portal_Bullet.h"
#include "PortalProject/PortalProjectCharacter.h"

// Sets default values
AFizzler::AFizzler()
	:
	RootComp(CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"))),
	Plane(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"))),
	Box(CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box Comp")))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(RootComp);
	Plane->SetupAttachment(RootComponent);
	Box->SetupAttachment(RootComponent);
}

void AFizzler::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어인지 확인
	if (APortalProjectCharacter* PC = Cast<APortalProjectCharacter>(OtherActor))
	{
		OnPlayerFizzled.Broadcast(PC->PlayerType);
	}

	if (APortal_Bullet* Bullet = Cast<APortal_Bullet>(OtherActor))
	{
		Bullet->Destroy();

		FCTween::Play(0, 1,
			[&](const float T) {
				PlaneMat->SetScalarParameterValue(TEXT("Intensity"), (10000 * T) + 100);
			},
			0.25f,
			EFCEase::Smoothstep)->SetYoyo(true);
	}
}

// Called when the game starts or when spawned
void AFizzler::BeginPlay()
{
	Super::BeginPlay();

	PlaneMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Plane->GetMaterial(0));
	Plane->SetMaterial(0, PlaneMat);
	Box->OnComponentBeginOverlap.AddDynamic(this, &AFizzler::BoxBeginOverlap);
}

