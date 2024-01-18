

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal_Screen.generated.h"

UCLASS()
class PORTALPROJECT_API APortal_Screen : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APortal_Screen();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComp;
};
