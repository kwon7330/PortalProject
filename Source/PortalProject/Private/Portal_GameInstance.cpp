


#include "Portal_GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"


UPortal_GameInstance::UPortal_GameInstance()
{
	
}

void UPortal_GameInstance::Init()
{
	Super::Init();
	auto Subsystem = IOnlineSubsystem::Get();
	if(Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,&UPortal_GameInstance::OnCreateSessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,&UPortal_GameInstance::OnFindSessionComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this,&UPortal_GameInstance::OnJoinSessionComplete);
	}
	
}

void UPortal_GameInstance::CreateMySession(const FString RoomName, const int32 PlayerCount)
{
	UE_LOG(LogTemp,Warning,TEXT("CreateMySession!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"))
	FOnlineSessionSettings SessionSettings;

	SessionSettings.bIsDedicated = false;
	FName SubsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	SessionSettings.bIsLANMatch = SubsysName == "NULL";

	SessionSettings.bShouldAdvertise = true;
	//SessionInterface->SendSessionInviteToFriend()
	//const FUniqueNetId& LocalUserId, FName SessionName, const FUniqueNetId& Friend
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinInProgress = false;
	SessionSettings.bAllowJoinViaPresence = false;
	SessionSettings.NumPublicConnections = PlayerCount;

	SessionSettings.Set(FName("Room_Name"),RoomName,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("Host_Name"),MySessionName,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	FUniqueNetIdPtr NetId = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	SessionInterface->CreateSession(*NetId,FName(MySessionName),SessionSettings);
	
}

void UPortal_GameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp,Warning,TEXT("OncreateSessionComplete!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"))
	if(bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("/Game/Portal/KSE/Maps/NewMap?Listen'"));
	}
}

void UPortal_GameInstance::FindOtherSession()
{
	UE_LOG(LogTemp,Warning,TEXT("FindingRoom@@@@@@@@@@@@@@@@@@@@@@@@"));
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE,true,EOnlineComparisonOp::Equals);
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSearch->MaxSearchResults = 5;
	SessionInterface->FindSessions(0,SessionSearch.ToSharedRef());
	
}

void UPortal_GameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	
	if(bWasSuccessful == false)
	{
		return;
	}
	auto Results = SessionSearch->SearchResults;
	for(int i = 0; i<Results.Num(); i++)
	{
		UE_LOG(LogTemp,Warning,TEXT("FindingRoomComplete@@@@@@@@@@@@@@@@@@@@@@@@"));
		auto SearchResults = Results[i];
		if(SearchResults.IsValid() == false)
		{
			continue;
		}
		FSessionInfo SessionInfo;
		SessionInfo.RoomIndex = i;
		FString RoomName;
		SearchResults.Session.SessionSettings.Get(FName("RoomName"),SessionInfo.RoomName);
		FString HostName;
		SearchResults.Session.SessionSettings.Get(FName("HostName"),SessionInfo.HostName);
		FString OwnerName = SearchResults.Session.OwningUserName;
		int32 MaxPlayerCount = SearchResults.Session.SessionSettings.NumPublicConnections;
		int32 CurrentPlayerCount = MaxPlayerCount - SearchResults.Session.NumOpenPublicConnections;
		SessionInfo.PlayerCount = FString::Printf(TEXT("(%d/%d)"),CurrentPlayerCount,MaxPlayerCount);
		OnSearchCompleted.Broadcast(SessionInfo);
	}
	OnSearchState.Broadcast(false);
}

void UPortal_GameInstance::JoinSelectedSession(int32 RoomIndex)
{
	UE_LOG(LogTemp,Warning,TEXT("JoinSelectedSession@@@@@@@@@@@@@@@@@@@@@@@@"));

	auto SearchResult = SessionSearch->SearchResults[RoomIndex];
	SessionInterface->JoinSession(0,FName(MySessionName),SearchResult);
}

void UPortal_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp,Warning,TEXT("OnjoinSessionComplete@@@@@@@@@@@@@@@@@@@@@@@@"));

	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Result Sucess @@@@@@@@@@@@@@@@@@@@@@@@"));

		FString URL;
		SessionInterface->GetResolvedConnectString(SessionName,URL);

		if(URL.IsEmpty() == false)
		{
			UE_LOG(LogTemp,Warning,TEXT("URL is not Empty @@@@@@@@@@@@@@@@@@@@@@@@"));

			auto PC = GetWorld()->GetFirstPlayerController();
			PC->ClientTravel(URL,TRAVEL_Absolute);
		}
	}
	
}


