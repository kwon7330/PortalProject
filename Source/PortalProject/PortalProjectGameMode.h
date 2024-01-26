// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PortalProjectGameMode.generated.h"

UCLASS(minimalapi)
class APortalProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APortalProjectGameMode();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	APlayerController* PBody;

	UPROPERTY()
	AActor* PBodyStart;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	APlayerController* Atlas;

	UPROPERTY()
	AActor* AtlasStart;
	

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
};



