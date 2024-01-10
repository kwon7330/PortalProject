

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal_PortalDemo.generated.h"

UCLASS()
class PORTALPROJECT_API APortal_PortalDemo : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APortal_PortalDemo();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	
};
