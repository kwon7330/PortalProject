// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UTextBlock;
class UScrollBox;
class UPortal_GameInstance;
class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class PORTALPROJECT_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void CreateRoom();
public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UButton* Btn_MultiPlay;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	// UButton* Btn_Quit;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	UButton* Btn_Back;

	UFUNCTION()
	void SwitchToMulti();
	UFUNCTION()
	void BackToMain();



	UPROPERTY()
	UPortal_GameInstance* GI;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget))
	UButton* Btn_NormalMulti;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget))
	UButton* Btn_CreateSession;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget))
	UButton* Btn_Back_1;
	UFUNCTION()
	void SwitchToInvite();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget))
	UScrollBox* Scroll_SessionList;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget))
	UButton* Btn_Find;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget))
	UTextBlock* txt_FindingMsg;
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<class USessionList> SessionList;

	UFUNCTION()
	void AddSlotWidget(const struct FSessionInfo& SessionInfo);

	UFUNCTION()
	void OnClickedFindSession();

	UFUNCTION()
	void OnChangeButtonEnabled(bool bIsSearching);
};
