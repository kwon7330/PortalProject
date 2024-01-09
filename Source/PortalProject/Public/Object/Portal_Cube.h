

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal_Cube.generated.h"

UCLASS()
class PORTALPROJECT_API APortal_Cube : public AActor
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
};
