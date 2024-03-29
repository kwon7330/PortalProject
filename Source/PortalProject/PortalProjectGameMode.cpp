// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortalProjectGameMode.h"

#include "PortalGameState.h"
#include "PortalProjectCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

APortalProjectGameMode::APortalProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Script/Engine.Blueprint'/Game/Portal/KHO/Test/BP_ThirdPersonCharacter_Child.BP_ThirdPersonCharacter_Child'"));

	if (PlayerPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;	
	}

	bUseSeamlessTravel = true;
}

void APortalProjectGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	//Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	// 이미 조종하는 Pawn이 있을때는 아무것도 안함
	if (NewPlayer->GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn exists? Pawn name: %s"), *NewPlayer->GetPawn()->GetActorNameOrLabel())
		return;
	}

	/*if (PBody && Atlas)
	{
		//UKismetSystemLibrary::QuitGame(GetWorld(), NewPlayer, EQuitPreference::Quit, false);
	}*/

	// PlayerStart 찾기
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	
	// 플레이어마다 스폰 포인트 다르게하기
	FTransform SpawnTransform;
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerStarts[0]->GetActorNameOrLabel())
	SpawnTransform = PlayerStarts[0]->GetActorTransform();
	PBodyStart = PlayerStarts[0];

	if (PBody && PlayerStarts.Num() > 1)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerStarts[1]->GetActorNameOrLabel())
		SpawnTransform = PlayerStarts[1]->GetActorTransform();
		AtlasStart = PlayerStarts[1];
	}
	
	SpawnTransform.SetScale3D(FVector::OneVector);
	
	APortalProjectCharacter* NewChar = GetWorld()->SpawnActorDeferred<APortalProjectCharacter>(DefaultPawnClass, SpawnTransform
		, NewPlayer, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	
	if (!PBody)
	{
		NewChar->PlayerType = EPlayerType::PBody;
		PBody = NewPlayer;
	}
	else
	{
		NewChar->PlayerType = EPlayerType::Atlas;
		Atlas = NewPlayer;
	}

	UGameplayStatics::FinishSpawningActor(NewChar, SpawnTransform);
	NewPlayer->Possess(NewChar);
}

AActor* APortalProjectGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{

	if (!AtlasStart || !PBodyStart)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

		AtlasStart = PlayerStarts[0];
		PBodyStart = PlayerStarts[1];
	}
	
	if (Player == Atlas)
	{
		check(AtlasStart);
		return AtlasStart;
	}

	check(PBodyStart);
	return PBodyStart;
}

APawn* APortalProjectGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	PRINTLOG(TEXT("SPAWN CALLED"))
	APawn* Pawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
	APortalProjectCharacter* Char = Cast<APortalProjectCharacter>(Pawn);

	check(Char);
	
	if (NewPlayer == Atlas)
	{
		Char->PlayerType = EPlayerType::Atlas;
	}
	else
	{
		Char->PlayerType = EPlayerType::PBody;
	}
	
	return Char;
}

