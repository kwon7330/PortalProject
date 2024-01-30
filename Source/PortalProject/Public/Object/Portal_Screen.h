

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal_Screen.generated.h"

class UBoxComponent;

UCLASS()
class PORTALPROJECT_API APortal_Screen : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APortal_Screen();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere)
	FRotator RotBase;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* TickTockSound;

	UPROPERTY(EditDefaultsOnly)
	UAudioComponent* AudioComponent;
	
	FRotator ActivatedRot;

	UFUNCTION()
	void ScreenMove();
	void ResetScreen();

	UFUNCTION(Server,Reliable)
	void ServerRPC_ScreenMove();
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_ScreenMove();
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_ResetScreen();
};
