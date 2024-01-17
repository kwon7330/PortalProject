

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Portal_Cube.generated.h"

class APortalProjectCharacter;

UCLASS()
class PORTALPROJECT_API APortal_Cube : public AActor, public IInteractable
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

	UFUNCTION()
	virtual void Interact_Implementation() override;

public:
	UPROPERTY()
	APortalProjectCharacter* Player;

	UPROPERTY()
	AActor* OwneddCube = nullptr;
	bool bAttached = false;

	bool bCheck = false;
	
	void Attached();
	void Detached();

	
};
