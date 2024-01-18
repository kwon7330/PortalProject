// Copyright Epic Games, Inc. All Rights Reserved.


#include "PortalProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Object/Portal_PortalManager.h"

void APortalProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}

	AActor* PortalManager = UGameplayStatics::GetActorOfClass(GetWorld(), APortal_PortalManager::StaticClass());
	check(PortalManager);
	PortalManager->SetOwner(this);
}

FMatrix APortalProjectPlayerController::GetCameraProjectionMatrix() const
{
	FMatrix ProjectionMatrix;

	if( GetLocalPlayer() != nullptr )
	{
		FSceneViewProjectionData PlayerProjectionData;

		GetLocalPlayer()->GetProjectionData( GetLocalPlayer()->ViewportClient->Viewport,
										PlayerProjectionData );

		ProjectionMatrix = PlayerProjectionData.ProjectionMatrix;
	}

	return ProjectionMatrix;
}
