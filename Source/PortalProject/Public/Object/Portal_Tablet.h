

#pragma once

#include "CoreMinimal.h"
#include "Portal_Bullet.h"
#include "GameFramework/Actor.h"
#include "Portal_Tablet.generated.h"

class UArrowComponent;
class UBoxComponent;

UCLASS()
class PORTALPROJECT_API APortal_Tablet : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APortal_Tablet();

protected:
	
	virtual void BeginPlay() override;

public:

	//=================================================================================================================================	

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="WallSettings")
	UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="WallSettings")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="WallSettings")
	UArrowComponent* SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class APortalActor* PortalActor;
};
