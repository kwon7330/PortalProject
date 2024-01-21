

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorSystem.generated.h"

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

	

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	
};
