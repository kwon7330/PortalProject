

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Portal_Turret.generated.h"


class UTurretFSM;

UCLASS()
class PORTALPROJECT_API APortal_Turret : public ACharacter
{
	GENERATED_BODY()

public:
	
	APortal_Turret();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//=====================================================================================================================
public:
	UPROPERTY(EditAnywhere,Category="AttackPoint")
	UArrowComponent* AttackPoint;
	
	
	UPROPERTY(EditAnywhere)
	UTurretFSM* TurretFsm;
	
//=====================================================================================================================

	

};
