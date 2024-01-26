// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PortalProject/PortalProjectCharacter.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class PORTALPROJECT_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* LeftPortalIndicator;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* RightPortalIndicator;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* DamageAnim;

	UFUNCTION()
	void PlayDamageAnim();

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* LeftBlank;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* LeftFull;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* RightBlank;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* RightFull;
	
	UFUNCTION()
	void AddedPortal(EPortalType Portal);
	
	UFUNCTION()
	void RemovedPortal(EPortalType Portal);

protected:
	virtual void NativePreConstruct() override;
};
