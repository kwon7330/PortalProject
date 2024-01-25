// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include "Portal_GameInstance.h"
#include "Components/Button.h"
#include "SessionList.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"


void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GI = Cast<UPortal_GameInstance>(GetWorld()->GetGameInstance());

	GI->OnSearchCompleted.AddDynamic(this,&ULobbyWidget::AddSlotWidget);
	GI->OnSearchState.AddDynamic(this,&ULobbyWidget::OnChangeButtonEnabled);
	Btn_Find->OnClicked.AddDynamic(this,&ULobbyWidget::OnClickedFindSession);
	Btn_MultiPlay->OnClicked.AddDynamic(this,&ULobbyWidget::SwitchToMulti);
	Btn_CreateSession->OnClicked.AddDynamic(this,&ULobbyWidget::CreateRoom);
	Btn_Back->OnClicked.AddDynamic(this,&ULobbyWidget::BackToMain);
	Btn_Back_1->OnClicked.AddDynamic(this,&ULobbyWidget::SwitchToMulti);
	//Btn_Quit->OnClicked.AddDynamic(this,&ULobbyWidget::QuitGame);
	Btn_NormalMulti->OnClicked.AddDynamic(this,& ULobbyWidget::SwitchToInvite);
	
}

void ULobbyWidget::CreateRoom()
{
	UE_LOG(LogTemp,Warning,TEXT("Clicked CreateRoom!!!!!!!!!!!!!!!!!!!!!"))
	if(GI)
	{
		FString RoomName = TEXT("PortalGame");
		int32 PlayerCount = 2;
		GI->CreateMySession(RoomName,PlayerCount);
	}
}

void ULobbyWidget::SwitchToMulti()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void ULobbyWidget::BackToMain()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}



void ULobbyWidget::SwitchToInvite()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);
	OnClickedFindSession();
}

void ULobbyWidget::AddSlotWidget(const FSessionInfo& SessionInfo)
{
	UE_LOG(LogTemp,Warning,TEXT("ADDSlotWidget!!!!!!!!!!!!!!!!!!"))

	auto Slots = CreateWidget<USessionList>(this,SessionList);
	Slots->Set(SessionInfo);
	Scroll_SessionList->AddChild(Slots);
}

void ULobbyWidget::OnClickedFindSession()
{
	UE_LOG(LogTemp,Warning,TEXT("OnClickedFIndSession!!!!!!!!!!!!!!!!!!"))

	Scroll_SessionList->ClearChildren();
	if(GI)
	{
		GI->FindOtherSession();
	}
}

void ULobbyWidget::OnChangeButtonEnabled(bool bIsSearching)
{
	UE_LOG(LogTemp,Warning,TEXT("OnchageButtonEnabled!!!!!!!!!!!!!!!!!!"))
	Btn_Find->SetIsEnabled(!bIsSearching);
	if(bIsSearching)
	{
		txt_FindingMsg->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		txt_FindingMsg->SetVisibility(ESlateVisibility::Hidden);
	}
}
