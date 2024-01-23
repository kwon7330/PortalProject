

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "SphereBallFactory.generated.h"

class AInteractButton;
class ASphereBall;
class UArrowComponent;

UCLASS()
class PORTALPROJECT_API ASphereBallFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASphereBallFactory();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;
	
	
	UPROPERTY(EditAnywhere)
	UArrowComponent* ArrowComp;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASphereBall> BallFactory;

	
	
	void FallingBall();


	UFUNCTION(Server,Reliable)
	void ServerRPC_FallingBall();

	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_FallingBall();


	void CheckButtonOverlap();
};
