// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPortalType : uint8
{
	Player1Blue,
	Player1Purple,
	Player2Orange,
	Player2Red,
};

static const TMap<EPortalType, FLinearColor> PortalColorMap = {
	{EPortalType::Player1Blue, FLinearColor(0, 0.04, 1, 1)},
	{EPortalType::Player1Purple, FLinearColor(0.94, 0, 1, 1)},
	{EPortalType::Player2Orange, FLinearColor(1, 0.34, 0.05, 1)},
	{EPortalType::Player2Red, FLinearColor(1, 0, 0, 1)}
};
