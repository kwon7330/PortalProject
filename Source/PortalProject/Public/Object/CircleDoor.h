

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
	
	
	void DoorOpen();
	void DoorClose();
};
