// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortalProjectCharacter.h"
#include "PortalProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AI/Portal_Turret.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Object/Portal_Cube.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APortalProjectCharacter

APortalProjectCharacter::APortalProjectCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(GetMesh());
	//(X=26.000000,Y=0.000000,Z=75.000000)
	CameraComp->SetRelativeLocation(FVector(26.0f, 0.f, 75.f));
	CameraComp->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	// Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	// Mesh1P->SetOnlyOwnerSee(true);
	// Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	// Mesh1P->bCastDynamicShadow = false;
	// Mesh1P->CastShadow = false;
	// //Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	// Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	PortalGun = CreateDefaultSubobject<UStaticMeshComponent>("PortalGun");
	
	AttachComp = CreateDefaultSubobject<USceneComponent>("AttachComp");
	AttachComp ->SetupAttachment(GetMesh());
	//(X=-0.000000,Y=101.808839,Z=135.134964)
	AttachComp-> SetRelativeLocation(FVector(0,101,135));

	// 플레이어 시점 고정.
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void APortalProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void APortalProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APortalProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APortalProjectCharacter::Look);

		// 큐브 Pickup
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Started, this, &APortalProjectCharacter::Pickup);
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Completed, this, &APortalProjectCharacter::Released);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
//=========================================================================================================================================
void APortalProjectCharacter::AttachCube(AActor* Cube)
{
	UE_LOG(LogTemp,Warning,TEXT("333333333333333333333333333333"));

	auto MeshComp = Cube->GetComponentByClass<UStaticMeshComponent>();
	MeshComp->SetSimulatePhysics(false);
	MeshComp->AttachToComponent(AttachComp,FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void APortalProjectCharacter::DetachCube(AActor* Cube)
{
	auto MeshComp = Cube->GetComponentByClass<UStaticMeshComponent>();
	MeshComp->SetSimulatePhysics(true);
	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
}

void APortalProjectCharacter::Pickup(const FInputActionValue& Value)
{
	UE_LOG(LogTemp,Warning,TEXT("11111111111111111111111111111"));

	// 오브젝트를 들고 있지 않다면, 일정 범위 안에 있는 큐브를 잡는다.
	if(bHasCube == true)
	{
		return;
	}
	// 필요 속성 : 큐브 여부, 큐브와의 거리
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),AllActors);
	
	for(auto TempCube : AllActors)
	{
		UE_LOG(LogTemp,Warning,TEXT("@@@@@@@@@@@@@@@@@@@@@@@@@@@@"));
		if(TempCube->GetName().Contains("BP_PortalCube") == false)
		{
				continue;
		}
		float Distance = FVector::Dist(GetActorLocation(),TempCube->GetActorLocation());
		if(Distance>PickCube)
		{
			continue;
		}
		UE_LOG(LogTemp,Warning,TEXT("22222222222222222222222222222222"));
		bHasCube = true;
		OwnedCube = TempCube;
		OwnedCube->SetOwner(this);

		AttachCube(OwnedCube);
		break;

		// for(auto TempCube : AllActors)
		// {
		// 	APortal_Cube* Cube = Cast<APortal_Cube>(TempCube);
		// 	if(Cube)
		// 	{
		// 		float Distance = FVector::Dist(GetActorLocation(),TempCube->GetActorLocation());
		// 		if(Distance>PickCube)
		// 		{
		// 			bHasCube = true;
		// 			OwnedCube = TempCube;
		// 			OwnedCube->SetOwner(this);
		// 			
		// 		}
		// 	}
		// }
	}
}

void APortalProjectCharacter::Released(const FInputActionValue& Value)
{
	// 오브젝트를 들면 키를 누루고 있는 동안 계속 들고 있고 키를 때면 내렿놓게 된다.
	if(bHasCube == false)
	{
		return;
	}
	if(OwnedCube)
	{
		DetachCube(OwnedCube);
		bHasCube = false;
		OwnedCube->SetOwner(nullptr);
		OwnedCube = nullptr;
		
	}
	
}

//=========================================================================================================================================

void APortalProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APortalProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APortalProjectCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool APortalProjectCharacter::GetHasRifle()
{
	return bHasRifle;
}