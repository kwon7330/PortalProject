

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "InteractButton.generated.h"

class AIndicatorLight;
class ASphereBallFactory;
class AMovingFloor;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FButtonStatusChanged, bool, bButtonStatus);

UCLASS()
class PORTALPROJECT_API AInteractButton : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AInteractButton();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	FButtonStatusChanged OnButtonStatusChanged;

	UPROPERTY(VisibleDefaultsOnly)
	UBoxComponent* Trigger;
	UPROPERTY(VisibleDefaultsOnly)
	USkeletalMeshComponent* ButtonMesh;

	UPROPERTY(EditAnywhere)
	ASphereBallFactory* BallFactory;
	UPROPERTY(EditAnywhere)
	AMovingFloor* MovingFloor; 


public:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server,Reliable)
	void ServerRPC_OnOverlap();
	UFUNCTION(Server,Reliable)
	void ServerRPC_EndOverlap();

	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_OnOverlap();

	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_EndOverlap();


	
	UPROPERTY(EditAnywhere)
	USoundBase* ButtonDownSound;
	UPROPERTY(EditAnywhere)
	USoundBase* ButtonUPSound;

	UPROPERTY(EditAnywhere)
	UAnimSequence* ButtonDownAnim;
	UPROPERTY(EditAnywhere)
	UAnimSequence* ButtonUpAnim;


	
	UPROPERTY(Replicated)
	bool bisOverlap;
};
