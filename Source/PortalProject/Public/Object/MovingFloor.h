

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "MovingFloor.generated.h"

UCLASS()
class PORTALPROJECT_API AMovingFloor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	
	AMovingFloor();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	virtual void ButtonInteract() override;

	UPROPERTY()
	UStaticMeshComponent* MeshComp;

	void FloorAct();
	void Return();

	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_FloorAct();
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_ReturnAct();
	
};
