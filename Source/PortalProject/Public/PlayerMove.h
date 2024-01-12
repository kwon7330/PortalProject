

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMove.generated.h"


class UInputMappingContext;
struct FInputActionValue;
class APortalProjectCharacter;
class UInputAction;
class UInputComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTALPROJECT_API UPlayerMove : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPlayerMove();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupPlayerInput(class UInputComponent* PlayerInputComponent);
	
public:
	UPROPERTY(EditAnywhere)
	APortalProjectCharacter* Self;
	
public:


	UPROPERTY()
	UInputMappingContext* IMC;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Inputs")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Inputs")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* LookAction;


	
protected:
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJump();
	
	
};
