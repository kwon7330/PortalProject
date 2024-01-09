

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurretFSM.generated.h"

class AAIController;
class APortal_Turret;

UENUM(BlueprintType)
enum class ETurretState : uint8
{
	Idle,
	Attack,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTALPROJECT_API UTurretFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UTurretFSM();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	APortal_Turret* Self;
	ACharacter* Target;

	AAIController* Ai;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="TurretSettings")
	ETurretState State;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="TurretSettings")
	float AttackTime = 3.0f;

	float CurrentTime = 0;
	
private:
	void TickIdle();
	void TickAttack();
	void TickDie();
	void RaserAttack();

public:
	void SetState(ETurretState Next);
};
