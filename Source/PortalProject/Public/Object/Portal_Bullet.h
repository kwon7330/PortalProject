

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../PortalProject.h"
#include "Portal_Bullet.generated.h"

class UNiagaraComponent;
class APortal_Tablet;
class APortal_PortalManager;
class APortal_PortalDemo;
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
	UPROPERTY(EditAnywhere, Category = "Bullet")
	UNiagaraComponent* TrailVfxComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* BurstVFX;
	
	UPROPERTY()
	APortal_PortalManager* PortalManager;
	
public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category="BulletTypeSettings")
	EPortalType Type;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;
};
