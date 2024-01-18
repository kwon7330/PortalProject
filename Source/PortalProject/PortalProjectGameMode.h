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

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	APlayerController* Atlas;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};



