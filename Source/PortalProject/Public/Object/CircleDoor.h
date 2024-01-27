

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CircleDoor.generated.h"

class UCircleDoorAnim;

UCLASS()
class PORTALPROJECT_API ACircleDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACircleDoor();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	UAnimSequence* OpenDoorAnim;

	UPROPERTY(EditAnywhere)
	UAnimSequence* CloseDoorAnim;

	UPROPERTY(EditAnywhere)
	USoundBase* DoorOpenSound;

	UPROPERTY(EditAnywhere)
	USoundBase* DoorCloseSound;
	
	void DoorOpen();
	void DoorClose();

	UFUNCTION(Server,Reliable)
	void ServerRPC_DoorOpen();
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_DoorOpen();

	UFUNCTION(Server,Reliable)
	void ServerRPC_DoorClose();
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_DoorClose();

	

};
