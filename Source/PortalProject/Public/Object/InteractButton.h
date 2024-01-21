

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractButton.generated.h"

class AMovingFloor;
class UBoxComponent;

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


	UPROPERTY(VisibleDefaultsOnly)
	UBoxComponent* Trigger;
	
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* ButtonMesh;

	

	
	UPROPERTY(EditAnywhere)
	AMovingFloor* MovingFloor; 
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server,Reliable)
	void ServerRPC_OnOverlap();
	UFUNCTION(Server,Reliable)
	void ServerRPC_EndOverlap();

};
