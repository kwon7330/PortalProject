

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallNest.generated.h"

class ASphereBall;
class UBoxComponent;

UCLASS()
class PORTALPROJECT_API ABallNest : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABallNest();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere)
	ASphereBall* Ball;
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
