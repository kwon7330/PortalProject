

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal_CloseBullet.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UCapsuleComponent;

UCLASS()
class PORTALPROJECT_API APortal_CloseBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APortal_CloseBullet();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category="Bullet")
	USphereComponent* SphereComp;
	
	UPROPERTY(EditAnywhere,Category="Bullet")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere,Category="Bullet")
	UProjectileMovementComponent* ProjectileMovementComp;

};
