

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Portal_GameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	int32 RoomIndex;
	UPROPERTY(BlueprintReadOnly)
	FString RoomName;
	UPROPERTY(BlueprintReadOnly)
	FString HostName;
	UPROPERTY(BlueprintReadOnly)
	FString PlayerCount;
};


// 세션 검색이 끝났을 때 호출 될 델리게이트

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSigniture,const FSessionInfo&, SessionInfo );

// 세션 검색 상태 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchStateSigniture, bool, bIsSearching);

/**
 * 
 */
UCLASS()
class PORTALPROJECT_API UPortal_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPortal_GameInstance();
	
	virtual  void Init() override;
	IOnlineSessionPtr SessionInterface;

	// SessionName
	FString MySessionName = "Wayne";

	void CreateMySession(const FString RoomName, const int32 PlayerCount);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	
	void FindOtherSession();
	void OnFindSessionComplete(bool bWasSuccessful);

	FSearchSigniture OnSearchCompleted;
	FSearchStateSigniture OnSearchState;
	
	
	void JoinSelectedSession(int32 RoomIndex);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
};
