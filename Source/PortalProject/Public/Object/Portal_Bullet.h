

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../PortalProject.h"
#include "Portal_Bullet.generated.h"

class UProjectileMovementComponent;
class USphereComponent;



UCLASS()
class PORTALPROJECT_API APortal_Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APortal_Bullet();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Bullet")
	USphereComponent* SphereComp;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Bullet")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Bullet")
	UProjectileMovementComponent* ProjectileMovementComp;

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="BulletTypeSettings")
	EPortalType Type;

	
	
};
