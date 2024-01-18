// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortalProjectCharacter.h"
#include "EngineUtils.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerMove.h"
#include "PlayerUI.h"
#include "PortalProjectPlayerController.h"
#include "AI/Portal_Turret.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Layers/LayersSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Object/Portal_Bullet.h"
#include "Object/Portal_Cube.h"
#include "Object/Portal_SmallButton.h"
#include "Object/Portal_Tablet.h"


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

	ArmComp = CreateDefaultSubobject<USpringArmComponent>("ArmComp");
	ArmComp -> SetupAttachment(CameraComp);
	ArmComp -> TargetArmLength = 35.f;
	//(X=-2.707669,Y=15.355950,Z=-8.469816)
	ArmComp -> SetRelativeLocation(FVector(-2.7f,15.f,-8.4f));
	ArmComp -> SetRelativeRotation(FRotator(0,0,169.f));
	
	
	PortalGun = CreateDefaultSubobject<UStaticMeshComponent>("PortalGun");
	PortalGun -> SetupAttachment(ArmComp);
	//(X=-38.824636,Y=-2.561624,Z=-0.000000)
	//(Pitch=0.000000,Yaw=-159.999999,Roll=0.000000)
	//0.1
	PortalGun->SetRelativeLocationAndRotation(FVector(-38.f,-2.5f,0.f),FRotator(0.f,-160.f,0.f));
	PortalGun->SetRelativeScale3D(FVector(0.1f));
	
	AttachComp = CreateDefaultSubobject<USceneComponent>("AttachComp");
	AttachComp ->SetupAttachment(PortalGun,FName(TEXT("FirePoint")));
	//(X=-0.000000,Y=101.808839,Z=135.134964)
	//(X=636.894156,Y=-55.584951,Z=-101.822835)
	AttachComp-> SetRelativeLocation(FVector(636.f,-55.f,-101.f));

	// 플레이어 시점 고정.
	bUseControllerRotationYaw = true;
	
	MoveComp = CreateDefaultSubobject<UPlayerMove>("MoveComp");

	bReplicates = true;
	SetReplicateMovement(true);
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
		
		GetMesh()->SetVisibleInSceneCaptureOnly(true);
	}

	// Create UI
	checkf(PlayerUIClass, TEXT("Player UI 클래스가 지정되지 않음"));
	PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), PlayerUIClass);
	PlayerUI->AddToViewport();
}

void APortalProjectCharacter::SetHasRifle(bool bNewHasRifle)
{
}

bool APortalProjectCharacter::GetHasRifle()
{
	return bHasRifle;
}

//////////////////////////////////////////////////////////////////////////// Input

void APortalProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		// 큐브 Pickup
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Started, this, &APortalProjectCharacter::Pickup);
		
		// 여는 포탈 날리기
		EnhancedInputComponent->BindAction(LeftClickShootAction, ETriggerEvent::Started, this, &APortalProjectCharacter::LeftClickPortal);
		// 닫는 포탈 날리기
		EnhancedInputComponent->BindAction(RightClickShootAction, ETriggerEvent::Started, this, &APortalProjectCharacter::RightClickPortal);
		MoveComp->SetupPlayerInput(PlayerInputComponent);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
//=========================================================================================================================================


void APortalProjectCharacter::Pickup(const FInputActionValue& Value)
{
	//E 키를 눌렀을 때 
	// if(isTakeCube == true && isPushButton == false)
	// {
	// 	// 큐브를 들고있다면
	// 	if(bHasCube == true)
	// 	{	UE_LOG(LogTemp,Warning,TEXT("bHasCubeTrue!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
	//
	// 		// 큐브를 놓고
	// 		
	// 	}
	// 	// 큐브를 들고있지않다면 	
	// 	else
	// 	{
	// 		UE_LOG(LogTemp,Warning,TEXT("bHasCubeFalse!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
	// 		// 큐브를 든다.
	// 		PickupCube();
	// 	}
	// }
	// else if(isTakeCube == false && isPushButton == true)
	// {
	// 	PushButton();
	// }
	CheckObject();
	
}



// void APortalProjectCharacter::AttachCube(AActor* Cube)
// {
// 	UE_LOG(LogTemp,Warning,TEXT("Attach Cube!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
// 	auto MeshComp = Cube->GetComponentByClass<UStaticMeshComponent>();
// 	MeshComp->SetSimulatePhysics(false);
// 	MeshComp->AttachToComponent(AttachComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
// }
//
// void APortalProjectCharacter::DetachCube(AActor* Cube)
// {
// 	UE_LOG(LogTemp,Warning,TEXT("Detach Cube!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
// 	auto MeshComp = Cube->GetComponentByClass<UStaticMeshComponent>();
// 	MeshComp->SetSimulatePhysics(true);
// 	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
// }


void APortalProjectCharacter::PickupCube()
{
	UE_LOG(LogTemp,Warning,TEXT("ItsPickupCube!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
	
}

void APortalProjectCharacter::ReleaseCube()
{
	UE_LOG(LogTemp,Warning,TEXT("Release~!!!!!!"))
	if(bHasCube == false)
	{
		return;
	}
	
	
}


//=================================================================================================================================	

void APortalProjectCharacter::LeftClickPortal(const FInputActionValue& Value)
{
	ServerRPC_LeftClick();
}

void APortalProjectCharacter::RightClickPortal(const FInputActionValue& Value)
{
	
	ServerRPC_RightClick();
	
}
//=================================================================================================================================	


// 라인트레이스를 사용하여 오브젝트 구분하는 함수 구현
void APortalProjectCharacter::CheckObject()
{
	ServerRPC_CheckObj();
}
//=================================================================================================================================	

void APortalProjectCharacter::PushButton()
{
	UE_LOG(LogTemp,Warning,TEXT("PushButton"));
	
}
//=================================================================================================================================	



void APortalProjectCharacter::MultiRPC_CheckObj_Implementation()
{
	
	
}


void APortalProjectCharacter::ServerRPC_CheckObj_Implementation()
{

	//UE_LOG(LogTemp,Warning,TEXT("dfasdfafafafdafsafasf"));
	// 플레이어 컨트롤러를 설정
	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 1);
	APlayerController* pc = Cast<APlayerController>(Controller);
	if(pc)
	{
		
		//auto PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		//auto PCM2 = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 1);
		FHitResult HitInfo;
		// 카메라의 위치를 시작지점
		FVector StartPoint = CameraComp->GetComponentLocation();
		// 카메라의 시작지점에서 카메라의 회전 백터에 힘 150을 더하여 끝지점을 정한다.
		FVector EndPoint = StartPoint + pc->GetControlRotation().Vector() * 150;
		FCollisionQueryParams Params;
		// 자신은 콜리전 무시
		Params.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo,StartPoint,EndPoint,ECC_Visibility,Params);
		DrawDebugLine(GetWorld(),StartPoint,EndPoint,FColor::Green,false,1,0,0);
		if(bHit)
		{
			UE_LOG(LogTemp,Warning,TEXT("dfasdfafafafdafsafasf"));
			// 충돌한 물체를 판단해서
			AActor* HitActor = HitInfo.GetActor();
			if(IInteractable* InteractActor = Cast<IInteractable>(HitActor))
			{
				isTakeCube = true;
				InteractActor->Interact(this);
			}
		}
	}	
}


//=================================================================================================================================	

void APortalProjectCharacter::ServerRPC_LeftClick_Implementation()
{
	MultiRPC_LeftClick();
}

void APortalProjectCharacter::MultiRPC_LeftClick_Implementation()
{
	// 만약 컴퍼넌트의 주인이 플레이어 1 이라면 파란 색 포탈을 생성하고, 주인이 플레이어 2라면 주황색 포탈을 생성한다.
	// 큐브를 들고있으면 총을 쏘지 못한다.
	if(bHasCube == false)
	{
		//ServerRPC_LeftClick();
		
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0);
		if(PlayerController != nullptr)
		{
			auto PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
			FHitResult HitInfo;
			FVector StartPoint = PCM->GetCameraLocation();
			FVector EndPoint = StartPoint + PlayerController->GetControlRotation().Vector() * 10000;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo,StartPoint,EndPoint,ECC_Visibility,Params);
			DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 2, 0, 1.0f);
			if(bHit == true)
			{
				auto Tablets = Cast<APortal_Tablet>(HitInfo.GetActor());
				if(Tablets != nullptr)
				{
					FTransform SpawnPoint = HitInfo.GetActor()->GetActorTransform();
					APortal_Bullet* Bullet = GetWorld()->SpawnActorDeferred<APortal_Bullet>(BulletFactory,SpawnPoint);
					Bullet->Type = EPortalType::Player1Blue;
					UGameplayStatics::FinishSpawningActor(Bullet, SpawnPoint);
				}
			}
		}
	}
}

void APortalProjectCharacter::ServerRPC_RightClick_Implementation()
{
	MultiRPC_RightClick();
}


void APortalProjectCharacter::MultiRPC_RightClick_Implementation()
{
	// 만약 컴퍼넌트의 주인이 플레이어 1 이라면 보라 색 포탈을 생성하고, 주인이 플레이어 2라면 붉은색 포탈을 생성한다.
	// 큐브를 들고있으면 총을 쏘지 못한다.
	if(bHasCube == false)
	{
		//ServerRPC_RightClick();
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0);
		if(PlayerController != nullptr)
		{
			auto PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
			FHitResult HitInfo;
			FVector StartPoint = PCM->GetCameraLocation();
			FVector EndPoint = StartPoint + PlayerController->GetControlRotation().Vector() * 10000;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo,StartPoint,EndPoint,ECC_Visibility,Params);
			DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 5, 0, 1.0f);
			if(bHit == true)
			{
				
				auto Tablets = Cast<APortal_Tablet>(HitInfo.GetActor());
				if(Tablets != nullptr)
				{
					FTransform SpawnPoint = HitInfo.GetActor()->GetActorTransform();
					APortal_Bullet* Bullet = GetWorld()->SpawnActorDeferred<APortal_Bullet>(BulletFactory,SpawnPoint);
					Bullet->Type = EPortalType::Player1Purple;
					UGameplayStatics::FinishSpawningActor(Bullet, SpawnPoint);
					
				}
			}
		}
	}
}



void APortalProjectCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APortalProjectCharacter, bHasCube);
	DOREPLIFETIME(APortalProjectCharacter,isTakeCube);
	//DOREPLIFETIME(APortalProjectCharacter,AttachComp);
	
	
}

//=========================================================================================================================================



void APortalProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//CheckObject();
}

//=================================================================================================================================	
