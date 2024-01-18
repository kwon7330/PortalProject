// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(PortalLog, Warning, All);

#define NETMODE (GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : \
GetNetMode() == ENetMode::NM_Standalone ? TEXT("Standalone") : TEXT("Server"))

#define CALLINFO (FString(__FUNCTION__) + TEXT(" (") + FString::FromInt(__LINE__) + TEXT(")"))

#define PRINTLOG(fmt, ...) \
UE_LOG(PortalLog, Warning, TEXT("[%s]%s %s"), NETMODE, *CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__))

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

UENUM(BlueprintType)
enum class EPlayerType : uint8
{
	PBody,
	Atlas
};