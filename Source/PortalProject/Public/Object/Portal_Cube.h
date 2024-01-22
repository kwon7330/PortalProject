

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "PortalableObject.h"
#include "GameFramework/Actor.h"
#include "Portal_Cube.generated.h"

class APortalProjectCharacter;

UCLASS()
class PORTALPROJECT_API APortal_Cube : public AActor, public IInteractable, public IPortalableObject
{
	GENERATED_BODY()
	
public:	
	
	APortal_Cube();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere,Category="PortalCube|Components")
	UStaticMeshComponent* MeshComp;
	
	virtual void Interact(APortalProjectCharacter* Character) override;

public:
	

	UPROPERTY(Replicated)
	bool bAttached = false;
	
	void Attached(APortalProjectCharacter* Charter);
	void Detached(APortalProjectCharacter* Character);

	UFUNCTION(Server,Reliable)
	void ServerRPC_Attached(APortalProjectCharacter* Character);
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_Attached(APortalProjectCharacter* Character);


	UFUNCTION(Server,Reliable)
	void ServerRPC_Detached(APortalProjectCharacter* Character);
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_Detached(APortalProjectCharacter* Character);
};
