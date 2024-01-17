// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Interactable.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PortalProjectCharacter.generated.h"

enum class EPortalType : uint8;
class USpringArmComponent;
class UPlayerMove;
class APortal_CloseBullet;
class APortal_Bullet;
class APortal_Tablet;
class APortal_Cube;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APortalProjectCharacter : public ACharacter,public IInteractable
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	// /** First person camera */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	// UInputAction* JumpAction;
	//
	// /** Move Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	// UInputAction* MoveAction;



	
public:
	APortalProjectCharacter();

protected:
	virtual void BeginPlay();

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

public:
	virtual void Tick(float DeltaSeconds) override;



protected:
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:
	
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	UPROPERTY(EditAnywhere,Category="Portal_Settings")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere,Category="Portal_Settings")
	USpringArmComponent* ArmComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Portal_Settings")
	UPlayerMove* MoveComp;

//=================================================================================================================================	


	UPROPERTY(EditAnywhere,Category="Portal_Input")
	UInputAction* PickUpAction;
	UPROPERTY(EditAnywhere,Category="Portal_Input")
	UInputAction* ReleaseAction;
	UPROPERTY(EditAnywhere,Category="Portal_Input")
	UInputAction* LeftClickShootAction;
	UPROPERTY(EditAnywhere,Category="Portal_Input")
	UInputAction* RightClickShootAction;

//=================================================================================================================================	

	// 큐브를 붙일 컴퍼넌트
public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PortalGun;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* AttachComp;

public:
	// 큐브를 들고 있는가?
	UPROPERTY(Replicated)
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
	// 큐브를 컴포넌트로 부터 때는 함수.
	void DetachCube(AActor* Cube);
	

	void PickupCube();
	void ReleaseCube();
	
//=================================================================================================================================	
	UPROPERTY(EditAnywhere)
	APortal_Tablet* PortalTablet;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APortal_Bullet> BulletFactory;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APortal_Bullet> CloseBulletFactory;

	UPROPERTY(EditAnywhere)
	APortal_Bullet* BulletType;

	
	void LeftClickPortal(const FInputActionValue& Value);
	void RightClickPortal(const FInputActionValue& Value);
	
	
//=================================================================================================================================	

	void CheckObject();

public:
	UPROPERTY(EditAnywhere)
	class APortal_SmallButton* SmallButton;


//=================================================================================================================================	

	
	// E키를 사용했을 때 버튼과 큐브를 누를 수 있도록 구분

	
	bool isPushButton = false;
	
	bool isTakeCube = false;
	
	
//=================================================================================================================================	

	// 버튼을 누르는 함수
	void PushButton();

//=================================================================================================================================	

	void RemovePortal(EPortalType OldPortalType);
	

//=================================================================================================================================	

	// 네트워크

	
	UFUNCTION(Client,Unreliable)
	void MultiRPC_CheckObj();


	
	UFUNCTION(Server,Reliable)
	void ServerRPC_LeftClick();
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_LeftClick();

	
	UFUNCTION(Server,Reliable)
	void ServerRPC_RightClick();
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_RightClick();


	UFUNCTION(Server,Reliable)
	void ServerRPC_PickupCube();
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_PickupCube(AActor* Cube);
	
	UFUNCTION(Server,Reliable)
	void ServerPRC_ReleaseCube();
	UFUNCTION(NetMulticast,Unreliable)
	void MultiRPC_ReleaseCube(AActor* Cube);
	
	
	
// =============== UI =========================
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UPlayerUI> PlayerUIClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	UPlayerUI* PlayerUI;
};

