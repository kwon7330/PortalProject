// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalProject/PortalProject.h"
#include "PortalActor.generated.h"

class UBoxComponent;
class UArrowComponent;
class APortal_PortalManager;

UCLASS()
class PORTALPROJECT_API APortalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY()
	APortal_PortalManager* PortalManager;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	EPortalType Type;

	UPROPERTY(ReplicatedUsing = "OnRep_LinkedPortal", EditAnywhere, BlueprintReadWrite, Category = "Portal|Variables")
	APortalActor* LinkedPortal;

	UFUNCTION()
	void OnRep_LinkedPortal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal|Settings")
	float OffsetAmount {-4.f};

	UPROPERTY()
	int32 CurrentRecursion {0};

	UPROPERTY(VisibleAnywhere)
	TMap<AActor*, float> RecentlyTeleported;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal|Settings")
	float TeleportCooldown {0.15f};

	UFUNCTION()
	void TickRecentlyTeleported(float DeltaTime);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal|Settings")
	int32 MaxRecursions {2};

	// 텔레포트 이후 밀어내는 값.
	UPROPERTY(EditAnywhere, Category = "Portal|Settings")
	float AfterTeleportVelocity {500};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal|Components")
	USceneComponent* RootComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal|Components")
	UStaticMeshComponent* PortalPlane;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal|Components")
	UArrowComponent* ForwardDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal|Components")
	UStaticMeshComponent* PortalEdgeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal|Components")
	UBoxComponent* ActorDetection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneCaptureComponent2D* PortalCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Portal|Settings")
	class UMaterialInterface* PortalBaseMat;

	UPROPERTY(VisibleAnywhere, Category = "Portal|Mat")
	class UMaterialInstanceDynamic* PortalMat;

	UPROPERTY(VisibleAnywhere, Category = "Portal|Mat")
	class UTextureRenderTarget2D* PortalRenderTarget;

	UPROPERTY(VisibleAnywhere, Category = "Portal|Mat")
	class UNiagaraComponent* PortalVfxComp;

	UPROPERTY(VisibleAnywhere, Category = "Portal")
	UBoxComponent* BacksideDetection;
	
	UPROPERTY(VisibleAnywhere, Category = "Portal")
	UBoxComponent* PlaneBox;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> CollisionIgnoreActors;
	
	UPROPERTY()
	FVector LastPosition;
	
	UPROPERTY()
	bool bLastInFront;
	
	UPROPERTY()
	UWorld* World;

	UFUNCTION()
	void OnPlaneBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlaneBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> DetectedActors;
	
	UFUNCTION()
	void OnActorDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnActorDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void LinkWithOtherPortal();
	void UnlinkPortal();
	
	void SetClipPlanes();
	
	void UpdateSceneCaptureRecursive(const FVector& InLocation, const FRotator& InRotation);
	FVector UpdateLocation(const FVector& OldLocation) const;
	FRotator UpdateRotation(const FRotator& OldRotation) const;
	FVector UpdateVelocity(const FVector& OldVelocity) const;
	void CheckViewportSize();
	void CheckIfShouldTeleport();
	bool CheckIfPointCrossingPortal(const FVector& Point, const FVector& PortalLocation, const FVector& PortalNormal);
	
	void TeleportChar(ACharacter* Char);
	void TeleportObject(AActor* Actor);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;
};
