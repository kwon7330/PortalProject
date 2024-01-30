// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortalProjectCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerMove.h"
#include "PlayerUI.h"
#include "PortalProjectGameMode.h"
#include "AI/Portal_Turret.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Object/Portal_Bullet.h"
#include "Object/Portal_Cube.h"
#include "Object/Portal_PortalManager.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APortalProjectCharacter




APortalProjectCharacter::APortalProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
	
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

	MeshGunComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Gun Comp"));
	MeshGunComponent->SetupAttachment(GetMesh(), FName("GunSocket"));
	
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
		MeshGunComponent->SetVisibleInSceneCaptureOnly(true);
		PortalGun->SetHiddenInSceneCapture(true);
	}
	else
	{
		PortalGun->SetVisibility(false);
	}

	// Create UI

	if (IsLocallyControlled())
	{
		checkf(PlayerUIClass, TEXT("Player UI 클래스가 지정되지 않음"));
		PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), PlayerUIClass);
		PlayerUI->AddToViewport();	
	}
	
	APortal_PortalManager* PM = Cast<APortal_PortalManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APortal_PortalManager::StaticClass()));
	if (PM && IsLocallyControlled())
	{
		PRINTLOG(TEXT("FOUND"))
		PM->OnPortalCreated.AddUniqueDynamic(this, &APortalProjectCharacter::OnPortalCreated);
		PM->OnPortalDestroyed.AddUniqueDynamic(this, &APortalProjectCharacter::OnPortalDestroyed);
	}

	MeshGunComponent->SetRelativeLocationAndRotation(
		FVector(-23.16, -1.87, -11.49),
		FRotator(-37.75, -152.96, -324.16));

	if (PlayerType == EPlayerType::Atlas)
	{
		GetMesh()->SetSkeletalMesh(AtlasMesh);
		GetMesh()->SetAnimClass(AtlasAnimBP->GetAnimBlueprintGeneratedClass());
		MeshGunComponent->SetRelativeLocationAndRotation(
			FVector(-28.26, -2.16, -0.68),
		FRotator(-29.48, -531.33, -995.88));
	}
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





//=================================================================================================================================	

void APortalProjectCharacter::LeftClickPortal(const FInputActionValue& Value)
{
	// 만약 컴퍼넌트의 주인이 플레이어 1 이라면 파란 색 포탈을 생성하고, 주인이 플레이어 2라면 주황색 포탈을 생성한다.
	// 큐브를 들고있으면 총을 쏘지 못한다.
	if(bHasCube == true)
	{
		return;
	}
	ServerRPC_LeftClick();
}

void APortalProjectCharacter::RightClickPortal(const FInputActionValue& Value)
{
	// 만약 컴퍼넌트의 주인이 플레이어 1 이라면 보라 색 포탈을 생성하고, 주인이 플레이어 2라면 붉은색 포탈을 생성한다.
	// 큐브를 들고있으면 총을 쏘지 못한다.

	if (bHasCube == true)
	{
		return;
	}
	ServerRPC_RightClick();
}



// 라인트레이스를 사용하여 오브젝트 구분하는 함수 구현
void APortalProjectCharacter::CheckObject()
{
	ServerRPC_CheckObject();
}


void APortalProjectCharacter::PushButton()
{
	UE_LOG(LogTemp,Warning,TEXT("PushButton"));
	
}

void APortalProjectCharacter::MultiRPC_LeftClick_Implementation()
{
	FVector SpawnSound = PortalGun->GetComponentLocation();
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(),ShootingSound1,SpawnSound);
}


void APortalProjectCharacter::ServerRPC_CheckObject_Implementation()
{
	// 플레이어 컨트롤러를 설정
	APlayerController* pc = Cast<APlayerController>(Controller);
	if(pc)
	{
		
		FHitResult HitInfo;
		// 카메라의 위치를 시작지점
		FVector StartPoint = CameraComp->GetComponentLocation();
		// 카메라의 시작지점에서 카메라의 회전 백터에 힘 150을 더하여 끝지점을 정한다.
		FVector EndPoint = StartPoint + pc->GetControlRotation().Vector() * 150;
		FCollisionQueryParams Params;
		// 자신은 콜리전 무시
		Params.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo,StartPoint,EndPoint,ECC_Visibility,Params);
		if(bHit)
		{
			// 충돌한 물체를 판단해서
			AActor* HitActor = HitInfo.GetActor();
			if(HitActor)
			{
				if(IInteractable* InteractActor = Cast<IInteractable>(HitActor))
				{
					
					isTakeCube = true;
					InteractActor->Interact(this);
				}
			}		
		}
	}
}

void APortalProjectCharacter::MultiRPC_RightClick_Implementation()
{
	FVector SpawnSound = PortalGun->GetComponentLocation();
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(),ShootingSound2,SpawnSound);
}

void APortalProjectCharacter::ShootBullet(bool bIsLeftClick)
{
	PlayShootAnim();
	
	APlayerController* PC = Cast<APlayerController>(Controller);
	check(PC);

	FTransform FirePoint;
	FirePoint.SetLocation(PortalGun->GetSocketLocation(FName("FirePoint")));
	FirePoint.SetRotation(PC->GetControlRotation().Quaternion());
	FirePoint.SetScale3D(FVector(0.1));
	
	APortal_Bullet* Bullet = GetWorld()->SpawnActorDeferred<APortal_Bullet>(BulletFactory, FirePoint, this, this);

	switch(PlayerType)
	{
	case EPlayerType::PBody:
		if (bIsLeftClick)
		{
			Bullet->Type = EPortalType::Player1Blue;	
		}
		else
		{
			Bullet->Type = EPortalType::Player1Purple;
		}
		break;
	case EPlayerType::Atlas:
		if (bIsLeftClick)
		{
			Bullet->Type = EPortalType::Player2Orange;	
		}
		else
		{
			Bullet->Type = EPortalType::Player2Red;
		}
		break;
	}
	
	UGameplayStatics::FinishSpawningActor(Bullet, FirePoint);
}

void APortalProjectCharacter::OnPortalCreated(EPortalType Type)
{
	if (!PlayerUI)
	{
		return;
	}
	
	switch (Type) {
	case EPortalType::Player1Blue:
	case EPortalType::Player1Purple:
		if (PlayerType != EPlayerType::PBody)
		{
			return;
		}
		break;
	case EPortalType::Player2Orange:
	case EPortalType::Player2Red:
		if (PlayerType != EPlayerType::Atlas)
		{
			return;
		}
		break;
	}
	
	PlayerUI->AddedPortal(Type);
}

void APortalProjectCharacter::OnPortalDestroyed(EPortalType Type)
{
	if (!PlayerUI)
	{
		return;
	}

	switch (Type) {
	case EPortalType::Player1Blue:
	case EPortalType::Player1Purple:
		if (PlayerType != EPlayerType::PBody)
		{
			return;
		}
		break;
	case EPortalType::Player2Orange:
	case EPortalType::Player2Red:
		if (PlayerType != EPlayerType::Atlas)
		{
			return;
		}
		break;
	}

	PlayerUI->RemovedPortal(Type);
}

void APortalProjectCharacter::SetHP(const float Value)
{
	if (HasAuthority() && Value < HP)
	{
		OnPortalCharDamaged();
	}

	HP = Value;
}

void APortalProjectCharacter::OnRep_HP()
{
	OnPortalCharDamaged();
}

void APortalProjectCharacter::OnPortalCharDamaged()
{
	if (IsLocallyControlled())
	{
		PlayerUI->PlayDamageAnim();
	}

	if (HP <= 0)
	{
		OnDeath();		
	}
}

void APortalProjectCharacter::OnDeath()
{
	APortal_PortalManager* Manager = Cast<APortal_PortalManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APortal_PortalManager::StaticClass()));
	if (Manager)
	{
		Manager->OnPlayerFizzled(PlayerType);
	}

	if (HasAuthority())
	{
		AController* PC = GetController();
		PC->UnPossess();
		GetWorld()->GetAuthGameMode<APortalProjectGameMode>()->RestartPlayer(PC);
	}
	
	Destroy();
}

void APortalProjectCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (UPrimitiveComponent* Root = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		Root->ClearMoveIgnoreActors();
		Root->ClearMoveIgnoreComponents();
	}
}

void APortalProjectCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller); IsLocallyControlled() && PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		
		GetMesh()->SetVisibleInSceneCaptureOnly(true);
	}

	// Create UI

	if (IsLocallyControlled() && !PlayerUI)
	{
		checkf(PlayerUIClass, TEXT("Player UI 클래스가 지정되지 않음"));
		PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), PlayerUIClass);
		PlayerUI->AddToViewport();	
	}
	
	APortal_PortalManager* PM = Cast<APortal_PortalManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APortal_PortalManager::StaticClass()));
	if (PM && IsLocallyControlled())
	{
		PRINTLOG(TEXT("FOUND"))
		PM->OnPortalCreated.AddUniqueDynamic(this, &APortalProjectCharacter::OnPortalCreated);
		PM->OnPortalDestroyed.AddUniqueDynamic(this, &APortalProjectCharacter::OnPortalDestroyed);
	}
}


void APortalProjectCharacter::ServerRPC_LeftClick_Implementation()
{
	ShootBullet(true);
	MultiRPC_LeftClick();
}

void APortalProjectCharacter::ServerRPC_RightClick_Implementation()
{
	ShootBullet(false);
	MultiRPC_RightClick();
}

void APortalProjectCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APortalProjectCharacter, bHasCube);
	DOREPLIFETIME(APortalProjectCharacter, PlayerType);
	DOREPLIFETIME(APortalProjectCharacter, HP);
}

void APortalProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//FString VelText = GetCharacterMovement()->GetLastUpdateVelocity().ToString();
	//GEngine->AddOnScreenDebugMessage(0, -1, FColor::White, VelText, true, FVector2D(2, 2));
}


