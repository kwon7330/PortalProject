// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Portal_SmallButton.h"

#include "Components/CapsuleComponent.h"
#include "PortalProject/PortalProjectCharacter.h"


APortal_SmallButton::APortal_SmallButton()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp -> SetCapsuleHalfHeight(70.f);
	CapsuleComp -> SetCapsuleRadius(35.f);

}


void APortal_SmallButton::BeginPlay()
{
	Super::BeginPlay();
	
}


void APortal_SmallButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}




