// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalActor.h"

#include "DummyPortal.h"
#include "FCTween.h"
#include "NiagaraComponent.h"
#include "PortalableObject.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Object/Portal_PortalManager.h"
#include "Object/Portal_Tablet.h"
#include "PortalProject/PortalProjectCharacter.h"
#include "PortalProject/PortalProjectPlayerController.h"

// Sets default values
APortalActor::APortalActor()
	:	RootComp(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"))),
		PortalPlane(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Plane Mesh"))),
		ForwardDirection(CreateDefaultSubobject<UArrowComponent>(TEXT("Forward Direction"))),
		PortalEdgeMesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Edge Mesh"))),
		ActorDetection(CreateDefaultSubobject<UBoxComponent>(TEXT("Actor Detection"))),
		PortalCamera(CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Portal Camera"))),
		PortalVfxComp(CreateDefaultSubobject<UNiagaraComponent>(TEXT("Portal VFX Comp"))),
		BacksideDetection(CreateDefaultSubobject<UBoxComponent>(TEXT("Backside Detection"))),
		PlaneBox(CreateDefaultSubobject<UBoxComponent>(TEXT("Plane Box")))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set up replication.
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	bAlwaysRelevant = true;
	
	// Set up attachments.
	SetRootComponent(RootComp);
	PortalPlane->SetupAttachment(RootComponent);
	PortalEdgeMesh->SetupAttachment(RootComponent);
	PortalCamera->SetupAttachment(RootComponent);
	ActorDetection->SetupAttachment(RootComponent);
	ForwardDirection->SetupAttachment(RootComponent);
	PortalVfxComp->SetupAttachment(RootComponent);
	BacksideDetection->SetupAttachment(RootComponent);
	PlaneBox->SetupAttachment(RootComponent);
	
	// Set tick group.
	SetTickGroup(TG_PostUpdateWork);

	PortalPlane->SetCastShadow(false);
	PortalPlane->bAffectDistanceFieldLighting = false;
	PortalPlane->bAffectDynamicIndirectLighting = false;
	PortalPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PortalEdgeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// Set up the scene capture 2D.
	PortalCamera->bOverride_CustomNearClippingPlane = true;
	PortalCamera->CustomNearClippingPlane = 1.f;

	PortalCamera->SetUsingAbsoluteScale(true);
	
	// Set the composite mode to composite for Recursive Rendering
	PortalCamera->CompositeMode = SCCM_Composite;
	PortalCamera->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
	PortalCamera->bCaptureEveryFrame = false;
	PortalCamera->bCaptureOnMovement = false;
	PortalCamera->bAlwaysPersistRenderingState = true;
	
	
}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	PRINTLOG(TEXT("BEGIN Owner: %s"), GetOwner() ? *GetOwner()->GetActorNameOrLabel(): TEXT("None"))
	Super::BeginPlay();

	World = GetWorld();
	
	PortalManager = Cast<APortal_PortalManager>(UGameplayStatics::GetActorOfClass(World, APortal_PortalManager::StaticClass()));
	check(PortalManager);

	// Set up the material and texture for the portal plane.
	check(PortalBaseMat);
	PortalMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(World, PortalBaseMat);
	PortalPlane->SetMaterial(0, PortalMat);
	
	const FVector2D ViewPortSize = UWidgetLayoutLibrary::GetViewportSize(World);
	PortalRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(World, (int32)ViewPortSize.X, (int32)ViewPortSize.Y);
	PortalRenderTarget->bAutoGenerateMips = false;
	PortalMat->SetTextureParameterValue(TEXT("Texture"), PortalRenderTarget);

	// Set edge mesh.
	UMaterialInstanceDynamic* EdgeMeshMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(World, PortalEdgeMesh->GetMaterial(0));
	EdgeMeshMat->SetVectorParameterValue(TEXT("PortalColor"), *PortalColorMap.Find(Type));
	PortalEdgeMesh->SetMaterial(0, EdgeMeshMat);

	PortalVfxComp->SetVariableLinearColor(FName("PortalColor"), *PortalColorMap.Find(Type));

	APortalProjectPlayerController* PPPC = Cast<APortalProjectPlayerController>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PPPC)
	{
		PortalCamera->bUseCustomProjectionMatrix = true;
		PortalCamera->CustomProjectionMatrix = PPPC->GetCameraProjectionMatrix();	
	}
	
	LinkWithOtherPortal();


	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]
	{
		TArray<AActor*> BacksideActors;
		BacksideDetection->GetOverlappingActors(BacksideActors);

		CollisionIgnoreActors = BacksideActors.FilterByPredicate([](const AActor* Actor)->bool
		{
			return Cast<APortal_Tablet>(Actor) || Cast<AStaticMeshActor>(Actor);
		});
	}));

	PlaneBox->OnComponentBeginOverlap.AddDynamic(this, &APortalActor::OnPlaneBoxBeginOverlap);
	PlaneBox->OnComponentEndOverlap.AddDynamic(this, &APortalActor::OnPlaneBoxEndOverlap);
	ActorDetection->OnComponentBeginOverlap.AddDynamic(this, &APortalActor::OnActorDetectionBeginOverlap);
	ActorDetection->OnComponentEndOverlap.AddDynamic(this, &APortalActor::OnActorDetectionEndOverlap);

	FCTween::Play(0, 1,
		[&](const float T)
		{
			SetActorScale3D(FVector::OneVector * T);
		},
		0.5);

	
	PRINTLOG(TEXT("END Owner: %s"), GetOwner() ? *GetOwner()->GetActorNameOrLabel(): TEXT("None"))
}

// Called every frame
void APortalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LinkedPortal)
	{
		UpdateSceneCaptureRecursive(FVector(), FRotator());
		CheckViewportSize();
		CheckIfShouldTeleport();
		//DrawDebugPoint(World, LinkedPortal->PortalCamera->GetComponentLocation(), 10, FColor::Red);
		//DrawDebugCamera(World, LinkedPortal->PortalCamera->GetComponentLocation(), LinkedPortal->PortalCamera->GetComponentRotation(),90.0, 2.f);
	}
	TickRecentlyTeleported(DeltaTime);
	CheckDetectedActors();
}

void APortalActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ResetCollisionIgnoredActors();
	
	FTransform SpawnTransform = GetActorTransform();
	SpawnTransform.SetScale3D(FVector::OneVector);
	ADummyPortal* Dummy = GetWorld()->SpawnActorDeferred<ADummyPortal>(DummyPortalClass, SpawnTransform, nullptr,
	                                                                   nullptr,
	                                                                   ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (Dummy)
	{
		Dummy->Type = Type;
		UGameplayStatics::FinishSpawningActor(Dummy, SpawnTransform);	
	}
	
	switch (Type) {
	case EPortalType::Player1Blue:
		if (PortalManager->PurplePortal)
		{
			PortalManager->PurplePortal->UnlinkPortal();
		}
		break;
	case EPortalType::Player1Purple:
		if (PortalManager->BluePortal)
		{
			PortalManager->BluePortal->UnlinkPortal();
		}
		break;
	case EPortalType::Player2Orange:
		if (PortalManager->RedPortal)
		{
			PortalManager->RedPortal->UnlinkPortal();
		}
		break;
	case EPortalType::Player2Red:
		if (PortalManager->OrangePortal)
		{
			PortalManager->OrangePortal->UnlinkPortal();
		}
		break;
	}

	PortalManager->OnPortalDestroyed.Broadcast(Type);
	PortalManager->BroadcastPortalDeleted(Type);
}

void APortalActor::OnRep_LinkedPortal()
{
	PRINTLOG(TEXT("LinkedPortalRepped"))
	LinkWithOtherPortal();
}

void APortalActor::TickRecentlyTeleported(float DeltaTime)
{
	for (auto Itr =RecentlyTeleported.CreateIterator(); Itr; ++Itr)
	{
		Itr->Value += DeltaTime;
		if (Itr->Value >= TeleportCooldown)
		{
			Itr.RemoveCurrent();
		}
	}
}

void APortalActor::OnPlaneBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!LinkedPortal)
	{
		return;
	}
	
	if (!OtherActor->Implements<UPortalableObject>())
	{
		return;
	}

	//PRINTLOG(TEXT("Begin Overlap %s"), *OtherActor->GetActorNameOrLabel());

	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
	if (PrimitiveComponent)
	{
		for (auto A: CollisionIgnoreActors)
		{
			UPrimitiveComponent* P = Cast<UPrimitiveComponent>(A->GetRootComponent());
			if (P)
			{
				P->IgnoreActorWhenMoving(OtherActor, true);
			}
			PrimitiveComponent->IgnoreActorWhenMoving(A, true);
		}

		if (PrimitiveComponent->IsSimulatingPhysics())
		{
			// if simulating physics,
			PrimitiveComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
		}
	}
	
}

void APortalActor::OnPlaneBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->Implements<UPortalableObject>())
	{
		return;
	}

	RecentlyTeleported.Remove(OtherActor);

	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
	if (PrimitiveComponent)
	{
		PrimitiveComponent->ClearMoveIgnoreActors();
		for (auto A: CollisionIgnoreActors)
		{
			UPrimitiveComponent* P = Cast<UPrimitiveComponent>(A->GetRootComponent());
			if (P)
			{
				P->IgnoreActorWhenMoving(OtherActor, false);
			}
		}
	}
}

void APortalActor::OnActorDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!LinkedPortal)
	{
		return;
	}

	if (!OtherActor->Implements<UPortalableObject>())
	{
		return;
	}
	//PRINTLOG(TEXT("%s"), *OtherActor->GetActorNameOrLabel())

	DetectedActors.AddUnique(OtherActor);
}

void APortalActor::OnActorDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DetectedActors.RemoveSwap(OtherActor);

	ResetCollisionIgnoredActors();
}

void APortalActor::LinkWithOtherPortal()
{
	if (!LinkedPortal)
	{
		return;
	}

	LinkedPortal->PortalCamera->TextureTarget = PortalRenderTarget;

	SetClipPlanes();

	if (!PortalMat)
	{
		return;
	}
	
	const FVector OffDist = ForwardDirection->GetForwardVector() * OffsetAmount;
	const FLinearColor OffsetDist = FLinearColor(OffDist.X, OffDist.Y, OffDist.Z, 0.f);
	PortalMat->SetVectorParameterValue(TEXT("OffsetDistance"), OffsetDist);
	PortalPlane->SetMaterial(0, PortalMat);
	PortalCamera->HiddenComponents.AddUnique(LinkedPortal->PortalVfxComp);
}

void APortalActor::UnlinkPortal()
{
	this->LinkedPortal = nullptr;
	PortalPlane->SetMaterial(0, PortalBaseMat);
	PortalCamera->ClearHiddenComponents();
}

void APortalActor::SetClipPlanes()
{
	if (!LinkedPortal)
	{
		return;
	}

	LinkedPortal->PortalCamera->bEnableClipPlane = true;

	constexpr int32 CLIPPLANEAMOUNT = -3;

	//UE_LOG(LogTemp, Warning, TEXT("Plane Vector: %.f %.f %.f"), PortalPlane->GetComponentLocation().X, PortalPlane->GetComponentLocation().Y, PortalPlane->GetComponentLocation().Z);
	//UE_LOG(LogTemp, Warning, TEXT("ForwardDirection Vector: %.f %.f %.f"), ForwardDirection->GetForwardVector().X, ForwardDirection->GetForwardVector().Y, ForwardDirection->GetForwardVector().Z);
	
	const FVector ClipPlaneBase = LinkedPortal->PortalPlane->GetComponentLocation() + (LinkedPortal->ForwardDirection->GetForwardVector() * CLIPPLANEAMOUNT);

	//UE_LOG(LogTemp, Warning, TEXT("CLIP BASE VECTOR: %.f %.f %.f"), ClipPlaneBase.X, ClipPlaneBase.Y, ClipPlaneBase.Z);
	LinkedPortal->PortalCamera->ClipPlaneBase = ClipPlaneBase;
	LinkedPortal->PortalCamera->ClipPlaneNormal = LinkedPortal->ForwardDirection->GetForwardVector();
}

void APortalActor::CheckDetectedActors()
{
	for (auto a: DetectedActors)
	{
		//float VelLength = a->GetVelocity().Length();
		float Veldot = ForwardDirection->GetForwardVector().Dot(a->GetVelocity());
		//PRINTLOG(TEXT("Length: %.f, Dot: %.f"), VelLength, Veldot);

		if (Veldot < -700)
		{
			//PRINTLOG(TEXT("VEL!"))
			for (auto b: CollisionIgnoreActors)
			{
				auto root = Cast<UPrimitiveComponent>(a->GetRootComponent());
				root->MoveIgnoreActors.Add(b);

				if (ACharacter* Char = Cast<ACharacter>(a))
				{
					PRINTLOG(TEXT("Adding Force!"))
					FVector TowardsPortal = ForwardDirection->GetComponentLocation() - b->GetActorLocation();
					TowardsPortal.Normalize();
					Char->GetCharacterMovement()->AddForce(TowardsPortal * 5000);
				}
			}

			CollisionModifiedActors.AddUnique(a);
		}
	}
}

void APortalActor::ResetCollisionIgnoredActors()
{
	for (auto a: CollisionModifiedActors)
	{
		for (auto b: CollisionIgnoreActors)
		{
			Cast<UPrimitiveComponent>(a->GetRootComponent())->IgnoreActorWhenMoving(b, false);
		}
	}
}

void APortalActor::UpdateSceneCaptureRecursive(const FVector& InLocation, const FRotator& InRotation)
{
	if (CurrentRecursion == 0)
	{
		APlayerCameraManager* CM = UGameplayStatics::GetPlayerCameraManager(World, 0);
		FVector TempLoc = UpdateLocation(CM->GetCameraLocation());
		FRotator TempRot = UpdateRotation(CM->GetCameraRotation());

		//UE_LOG(LogTemp, Warning, TEXT("Temp: %.f %.f %.f"), TempLoc.X, TempLoc.Y, TempLoc.Z);
		//UE_LOG(LogTemp, Warning, TEXT("Temp2: %.f %.f %.f"), Temp2Loc.X, Temp2Loc.Y, Temp2Loc.Z);
		
		CurrentRecursion++;

		UpdateSceneCaptureRecursive(TempLoc, TempRot);

		LinkedPortal->PortalCamera->SetWorldLocationAndRotation(TempLoc, TempRot);
		LinkedPortal->PortalCamera->CaptureScene();

		CurrentRecursion = 0;
	}
	else
	{
		if (CurrentRecursion < MaxRecursions)
		{
			FVector TempLoc = UpdateLocation(InLocation);
			FRotator TempRot = UpdateRotation(InRotation);

			CurrentRecursion++;
			
			UpdateSceneCaptureRecursive(TempLoc, TempRot);

			LinkedPortal->PortalCamera->SetWorldLocationAndRotation(TempLoc, TempRot);
			LinkedPortal->PortalCamera->CaptureScene();
		}
		else
		{
			LinkedPortal->PortalCamera->SetWorldLocationAndRotation(UpdateLocation(InLocation),
				UpdateRotation(InRotation));
			PortalPlane->SetVisibility(false);
			LinkedPortal->PortalCamera->CaptureScene();
			PortalPlane->SetVisibility(true);
		}
	}
}

FVector APortalActor::UpdateLocation(const FVector& OldLocation) const
{
	const FTransform SelfTransform = GetActorTransform();
	const FVector SelfScale = SelfTransform.GetScale3D();
	const FVector LinkedScale = FVector(SelfScale.X * -1, SelfScale.Y * -1, SelfScale.Z);
	const FTransform LinkedTransform = FTransform(SelfTransform.Rotator(), SelfTransform.GetLocation(), LinkedScale);

	const FVector Transformed = UKismetMathLibrary::InverseTransformLocation(LinkedTransform, OldLocation);
	return UKismetMathLibrary::TransformLocation(LinkedPortal->GetActorTransform(), Transformed);
}

FRotator APortalActor::UpdateRotation(const FRotator& OldRotation) const
{
	FVector X;
	FVector Y;
	FVector Z;
	
	UKismetMathLibrary::BreakRotIntoAxes(OldRotation, X, Y, Z);

	FVector XDir;
	FVector YDir;
	FVector ZDir;

	XDir = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), X);
	XDir = UKismetMathLibrary::MirrorVectorByNormal(XDir, FVector(1, 0, 0));
	XDir = UKismetMathLibrary::MirrorVectorByNormal(XDir, FVector(0, 1, 0));
	XDir = UKismetMathLibrary::TransformDirection(LinkedPortal->GetActorTransform(), XDir);

	YDir = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), Y);
	YDir = UKismetMathLibrary::MirrorVectorByNormal(YDir, FVector(1, 0, 0));
	YDir = UKismetMathLibrary::MirrorVectorByNormal(YDir, FVector(0, 1, 0));
	YDir = UKismetMathLibrary::TransformDirection(LinkedPortal->GetActorTransform(), YDir);

	ZDir = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), Z);
	ZDir = UKismetMathLibrary::MirrorVectorByNormal(ZDir, FVector(1, 0, 0));
	ZDir = UKismetMathLibrary::MirrorVectorByNormal(ZDir, FVector(0, 1, 0));
	ZDir = UKismetMathLibrary::TransformDirection(LinkedPortal->GetActorTransform(), ZDir);

	return UKismetMathLibrary::MakeRotationFromAxes(XDir, YDir, ZDir);
}

FVector APortalActor::UpdateVelocity(const FVector& OldVelocity) const
{
	PRINTLOG(TEXT("OLD VECTOR: %1.f, %1.f, %1.f"), OldVelocity.X, OldVelocity.Y, OldVelocity.Z)
	
	FVector Normalized = OldVelocity.GetSafeNormal();

	PRINTLOG(TEXT("NORMAL: %1.f, %1.f, %1.f"), Normalized.X, Normalized.Y, Normalized.Z);
	
	Normalized = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), Normalized);
	Normalized = UKismetMathLibrary::MirrorVectorByNormal(Normalized, FVector(1, 0, 0));
	Normalized = UKismetMathLibrary::MirrorVectorByNormal(Normalized, FVector(0, 1, 0));
	Normalized = UKismetMathLibrary::TransformDirection(LinkedPortal->GetActorTransform(), Normalized);

	
	return OldVelocity.Length() * Normalized;
}

void APortalActor::CheckViewportSize()
{
	if (!PortalRenderTarget)
	{
		return;
	}

	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(World);

	if (PortalRenderTarget->SizeX == ViewportSize.X && PortalRenderTarget->SizeY == ViewportSize.Y)
	{
		return;
	}

	UE_LOG(LogTemp,Warning, TEXT("Resizing"))
	PortalRenderTarget->ResizeTarget(ViewportSize.X, ViewportSize.Y);
}

void APortalActor::CheckIfShouldTeleport()
{
	TArray<AActor*> OverlappingActors; 
	ActorDetection->GetOverlappingActors(OverlappingActors);

	for (auto Itr = OverlappingActors.CreateIterator(); Itr; ++Itr)
	{
		if (!(*Itr)->Implements<UPortalableObject>())
		{
			Itr.RemoveCurrentSwap();
		}
	}
	
	if (OverlappingActors.IsEmpty())
	{
		return;
	}

	for (AActor* OverlappedActor : OverlappingActors)
	{
		if (RecentlyTeleported.Contains(OverlappedActor))
		{
			return;
		}
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *OverlappingActors[0]->GetActorNameOrLabel())
		FVector ActorLoc = OverlappedActor->GetActorLocation();
		if (APortalProjectCharacter* Chara = Cast<APortalProjectCharacter>(OverlappedActor))
		{
			bool bCross = CheckIfPointCrossingPortal(ActorLoc, PortalPlane->GetComponentLocation() + ForwardDirection->GetForwardVector() * 2, ForwardDirection->GetForwardVector());

			if (bCross)
			{
				TeleportChar(Chara);
			}
			
			ActorLoc = Chara->CameraComp->GetComponentLocation();
		}
		
		bool bCross = CheckIfPointCrossingPortal(ActorLoc, PortalPlane->GetComponentLocation(), ForwardDirection->GetForwardVector());
		if (bCross)
		{
			ACharacter* Char = Cast<ACharacter>(OverlappedActor);
			if (Char)
			{
				TeleportChar(Char);
			}
			else
			{
				TeleportObject(OverlappedActor);
			}
		}
	}
}

bool APortalActor::CheckIfPointCrossingPortal(const FVector& Point, const FVector& PortalLocation,
	const FVector& PortalNormal)
{
	bool bIsInFront = PortalNormal.Dot(Point - PortalLocation) >= 0;
	const FPlane Plane = UKismetMathLibrary::MakePlaneFromPointAndNormal(PortalLocation, PortalNormal);

	float T;
	FVector Intersection;
	
	bool bIntersected = UKismetMathLibrary::LinePlaneIntersection(LastPosition, Point, Plane, T, Intersection);

	// Was crossed the portal plane, is behind the plane, was front of the plane last frame?
	bool bInCrossing = bIntersected && !bIsInFront && bLastInFront;
	bLastInFront = bIsInFront;
	LastPosition = Point;

	return bInCrossing;
}

void APortalActor::TeleportChar(ACharacter* Char)
{

	//PRINTLOG(TEXT("Teleported"))
	
	// Teleport the character.
	if (Char->GetVelocity().Size() > 601)
	{
		Char->SetActorLocationAndRotation(LinkedPortal->ForwardDirection->GetComponentLocation() +
			LinkedPortal->ForwardDirection->GetForwardVector() * 2, UpdateRotation(Char->GetActorRotation()),
			false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		Char->SetActorLocationAndRotation(UpdateLocation(Char->GetActorLocation()), UpdateRotation(Char->GetActorRotation()), false, nullptr, ETeleportType::TeleportPhysics);
	}
	
	RecentlyTeleported.Add(Char, 0.f);
	LinkedPortal->RecentlyTeleported.Add(Char, 0.f);
	
	// Set the new control rotation.
	APlayerController* Cont = Cast<APlayerController>(Char->GetController());
	if (Cont)
	{
		Cont->SetControlRotation(UpdateRotation(Cont->GetControlRotation()));
		FRotator InitialRot = Cont->GetControlRotation();
		Cont->SetControlRotation(FRotator(InitialRot.Pitch, InitialRot.Yaw, 0));
	}	

	// Update the velocity.

	check(LinkedPortal)
	auto MoveComp = Char->GetCharacterMovement();
	//FVector NewVelocity = UpdateVelocity(Char->GetMovementComponent()->Velocity) +
	//	LinkedPortal->ForwardDirection->GetForwardVector() * AfterTeleportVelocity;

	FVector PreVelocity = MoveComp->GetLastUpdateVelocity();
	if (PreVelocity.IsNearlyZero())
	{
		PreVelocity = FVector(0, 0, -3900);
	}
	
	FVector UpdatedVelocity = UpdateVelocity(PreVelocity);
	PRINTLOG(TEXT("Updated Velocity: %.1f, %.1f, %.1f"), UpdatedVelocity.X, UpdatedVelocity.Y, UpdatedVelocity.Z)

	FVector TV = LinkedPortal->ForwardDirection->GetForwardVector() * AfterTeleportVelocity;
	PRINTLOG(TEXT("TVelocity: %.1f %.1f %.1f"), TV.X, TV.Y, TV.Z);

	FVector NewVelocity = UpdatedVelocity + TV;
	NewVelocity = NewVelocity.BoundToCube(3900.f);
	PRINTLOG(TEXT("New Velocity: %.1f %.1f %.1f"), NewVelocity.X, NewVelocity.Y, NewVelocity.Z);
	
	MoveComp->Velocity = NewVelocity;
	MoveComp->UpdateComponentVelocity();

	FVector NewVel = MoveComp->Velocity;
	PRINTLOG(TEXT("New MoveComp Velocity: %.1f %.1f %.1f"), NewVel.X, NewVel.Y, NewVel.Z);

	
	// TODO: Smooth Orientation
	// Orient the player.
	Char->GetCapsuleComponent()->SetWorldRotation(FRotator::ZeroRotator);

	// Cut this frame
	UGameplayStatics::GetPlayerCameraManager(World, 0)->SetGameCameraCutThisFrame();
	//PortalCamera->bCameraCutThisFrame = true;
}

void APortalActor::TeleportObject(AActor* Actor)
{
	FVector BeforeVelocity = Actor->GetVelocity();
	Actor->SetActorLocationAndRotation(UpdateLocation(Actor->GetActorLocation()), UpdateRotation(Actor->GetActorRotation()));

	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Actor->GetRootComponent());

	if (PrimitiveComponent)
	{
		PrimitiveComponent->AddImpulse(UpdateVelocity(BeforeVelocity) * 1.f +
		LinkedPortal->ForwardDirection->GetForwardVector() * AfterTeleportVelocity);
	}
}

void APortalActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APortalActor, LinkedPortal);
	DOREPLIFETIME(APortalActor, Type);
}

