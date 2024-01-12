

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
	
	virtual void Tick(float DeltaTime) override;

//=================================================================================================================================	

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="WallSettings")
	UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="WallSettings")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="WallSettings")
	UArrowComponent* SpawnPoint;
	
	
//=================================================================================================================================	

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="WallSettings")
	TSubclassOf<class APortal_Cube>SpawnFactory;

	// 포탈 총알이 부딪히면 포탈을 고정된 공간에 생성하고싶다.
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SpawnPortal();

	UPROPERTY()
	APortal_Bullet* Bullet;
	
};
