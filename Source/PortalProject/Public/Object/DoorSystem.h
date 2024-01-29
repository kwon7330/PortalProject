

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorSystem.generated.h"

class ACircleDoor;
class ADoorActor;
class UBoxComponent;

UCLASS()
class PORTALPROJECT_API ADoorSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ADoorSystem();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere)
	ADoorActor* Door;

	UPROPERTY(EditAnywhere)
	ACircleDoor* CircleDoor;


	

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



	UPROPERTY(Replicated)
	TArray<class APortalProjectCharacter*> CheckBox;

	UFUNCTION(Server,Reliable)
	void ServerRPC_DoorOpen();

	UFUNCTION(Server,Reliable)
	void ServerRPC_DoorClose();

	UFUNCTION(Server,Reliable)
	void ServerRPC_CircleDoorOpen();
	
};
