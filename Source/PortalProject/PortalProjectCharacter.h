// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PortalProjectCharacter.generated.h"

class APortal_Cube;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APortalProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	

	
public:
	APortalProjectCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UPROPERTY(EditAnywhere,Category="Portal_Settings")
	UCameraComponent* CameraComp;
	
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


//=================================================================================================================================	


	UPROPERTY(EditAnywhere,Category="Portal_Input")
	UInputAction* PickUpAction;
	
	// 큐브를 붙일 컴퍼넌트
public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PortalGun;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* AttachComp;

public:
	// 큐브를 들고 있는가?
	bool bHasCube = false;
	// 잡을 수 있는 범위
	UPROPERTY(EditAnywhere,Category="Cube")
	float PickCube = 100;

	// 들고 있는 큐브
	UPROPERTY(EditAnywhere)
	AActor* OwnedCube = nullptr;

	// 큐브를 잡기 처리할 함수.
	void Pickup(const FInputActionValue& Value);
	// 큐브를 컴포넌트로 붙이는 함수.
	void AttachCube(AActor* Cube);
	// 큐브를 놓기를 처리할 함수.
	void Released(const FInputActionValue& Value);
	// 큐브를 컴포넌트로 부터 때는 함수.
	void DetachCube(AActor* Cube);
	
//=================================================================================================================================	
	

};

