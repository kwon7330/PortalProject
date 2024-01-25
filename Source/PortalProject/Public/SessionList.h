// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionList.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PORTALPROJECT_API USessionList : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	int32 SessionNumber = 0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UTextBlock* txt_SessionNumber;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UTextBlock* txt_RoomName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UTextBlock* txt_HostName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UButton* Btn_Join;

	
	void Set(const struct FSessionInfo& SessionInfo);

	UFUNCTION()
	void JoinSession();

	
};
