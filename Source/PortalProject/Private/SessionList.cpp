// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionList.h"

#include "Components/TextBlock.h"
#include "Portal_GameInstance.h"
#include "Components/Button.h"

void USessionList::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Join->OnClicked.AddDynamic(this,&USessionList::JoinSession);
	
	
}

void USessionList::Set(const FSessionInfo& SessionInfo)
{
	txt_SessionNumber->SetText(FText::FromString(FString::Printf(TEXT("%d"),SessionInfo.RoomIndex)));
	txt_RoomName->SetText(FText::FromString(SessionInfo.RoomName));
	txt_HostName->SetText(FText::FromString(SessionInfo.HostName));

	SessionNumber = SessionInfo.RoomIndex;
}

void USessionList::JoinSession()
{
	UE_LOG(LogTemp,Warning,TEXT("JoinSession@@@@@@@@@@@@@@@@@@@@@@@@"));

	// 맵이 로딩 되는동안 실행될 UI를 넣는다.
	
	UE_LOG(LogTemp,Warning,TEXT("JoinSession"));
	auto GI = Cast<UPortal_GameInstance>(GetWorld()->GetGameInstance());
	if(GI)
	{
		GI->JoinSelectedSession(SessionNumber);
	}
}
