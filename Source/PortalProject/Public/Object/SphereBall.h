

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "SphereBall.generated.h"

UCLASS()
class PORTALPROJECT_API ASphereBall : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	
	ASphereBall();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SphereMeshComp;

	UFUNCTION()
	virtual void Interact(APortalProjectCharacter* Character) override;


	UPROPERTY(Replicated)
	bool bAttached = false;
	
	void SpAttached(APortalProjectCharacter* Character);
	void SpDetached(APortalProjectCharacter* Character);

	UFUNCTION(Server,Reliable)
	void ServerRPC_SpAttached(APortalProjectCharacter* Character);

	UFUNCTION(Server,Reliable)
	void ServerRPC_SpDetached(APortalProjectCharacter* Character);
	

	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_SpAttached(APortalProjectCharacter* Character);
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_SpDetached(APortalProjectCharacter* Character);
};
