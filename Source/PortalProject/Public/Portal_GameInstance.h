// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Portal_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTALPROJECT_API UPortal_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	// 게임 인스턴스에서 포탈이 존재하는지 확인하고 중복되는 포탈 있으면 기존에 있던 포탈을 제거하고, 새로운 포탈을 새로운 위치에 스폰 하고싶다.

	void CheckPortal();
};
