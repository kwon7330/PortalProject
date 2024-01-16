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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Portal|Settings")
	TSubclassOf<AActor> PortalableActorSubclassOf;
	
	UPROPERTY()
	APortal_PortalManager* PortalManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EPortalType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal|Variables")
	APortalActor* LinkedPortal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal|Settings")
	float OffsetAmount {-4.f};

	UPROPERTY()
	int32 CurrentRecursion {0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal|Settings")
	int32 MaxRecursions {3};
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UMaterialInterface* PortalBaseMat;

	UPROPERTY(VisibleAnywhere, Category = "Portal|Mat")
	class UMaterialInstanceDynamic* PortalMat;

	UPROPERTY(VisibleAnywhere, Category = "Portal|Mat")
	class UTextureRenderTarget2D* PortalRenderTarget;

	UPROPERTY()
	FVector LastPosition;

	UPROPERTY()
	bool bLastInFront;
	
	UPROPERTY()
	UWorld* World;

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
	void TeleportChar();
};
