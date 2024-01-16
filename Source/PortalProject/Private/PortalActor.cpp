// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalActor.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Object/Portal_PortalManager.h"

// Sets default values
APortalActor::APortalActor()
	:	RootComp(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"))),
		PortalPlane(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Plane Mesh"))),
		ForwardDirection(CreateDefaultSubobject<UArrowComponent>(TEXT("Forward Direction"))),
		PortalEdgeMesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Edge Mesh"))),
		ActorDetection(CreateDefaultSubobject<UBoxComponent>(TEXT("Actor Detection"))),
		PortalCamera(CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Portal Camera")))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up attachments.
	SetRootComponent(RootComp);
	PortalPlane->SetupAttachment(RootComponent);
	PortalEdgeMesh->SetupAttachment(RootComponent);
	PortalCamera->SetupAttachment(RootComponent);
	ActorDetection->SetupAttachment(RootComponent);
	ForwardDirection->SetupAttachment(RootComponent);
	
	// Set tick group.
	SetTickGroup(TG_PostUpdateWork);

	PortalPlane->SetCastShadow(false);
	PortalPlane->bAffectDistanceFieldLighting = false;
	PortalPlane->bAffectDynamicIndirectLighting = false;
	PortalPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// Set up the scene capture 2D.
	PortalCamera->bOverride_CustomNearClippingPlane = true;
	PortalCamera->CustomNearClippingPlane = 1.f;
	
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
	Super::BeginPlay();

	World = GetWorld();
	
	PortalManager = Cast<APortal_PortalManager>(UGameplayStatics::GetActorOfClass(World, APortal_PortalManager::StaticClass()));
	check(PortalManager);

	// Set up the material and texture for the portal plane.
	check(PortalBaseMat);
	PortalMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(World, PortalBaseMat);
	PortalPlane->SetMaterial(0, PortalMat);

	const FVector2D ViewPortSize = UWidgetLayoutLibrary::GetViewportSize(World);
	PortalRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(World, (int32)ViewPortSize.X * 2,
		(int32)ViewPortSize.Y * 2);
	PortalRenderTarget->bAutoGenerateMips = true;
	PortalMat->SetTextureParameterValue(TEXT("Texture"), PortalRenderTarget);

	// Set edge mesh.
	UMaterialInstanceDynamic* EdgeMeshMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(World, PortalEdgeMesh->GetMaterial(0));
	EdgeMeshMat->SetVectorParameterValue(TEXT("PortalColor"), *PortalColorMap.Find(Type));
	PortalEdgeMesh->SetMaterial(0, EdgeMeshMat);
	
	LinkWithOtherPortal();
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
}

void APortalActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	switch (Type) {
	case EPortalType::Player1Blue:
		if (PortalManager->OrangePortal)
		{
			PortalManager->OrangePortal->UnlinkPortal();
		}
		break;
	case EPortalType::Player1Purple:
		if (PortalManager->BluePortal)
		{
			PortalManager->BluePortal->UnlinkPortal();
		}
		break;
	case EPortalType::Player2Orange:
		// TODO
		break;
	case EPortalType::Player2Red:
		// TODO
		break;
	}
}

void APortalActor::LinkWithOtherPortal()
{
	if (!LinkedPortal)
	{
		return;
	}

	LinkedPortal->PortalCamera->TextureTarget = PortalRenderTarget;

	SetClipPlanes();
	const FVector OffDist = ForwardDirection->GetForwardVector() * OffsetAmount;
	const FLinearColor OffsetDist = FLinearColor(OffDist.X, OffDist.Y, OffDist.Z, 0.f);
	PortalMat->SetVectorParameterValue(TEXT("OffsetDistance"), OffsetDist);
}

void APortalActor::UnlinkPortal()
{
	this->LinkedPortal = nullptr;
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
	FVector Normalized = OldVelocity.GetSafeNormal();
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
	ActorDetection->GetOverlappingActors(OverlappingActors, PortalableActorSubclassOf);
	
	if (OverlappingActors.IsEmpty())
	{
		return;
	}

	if (OverlappingActors[0])
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *OverlappingActors[0]->GetActorNameOrLabel())
		bool bCross = CheckIfPointCrossingPortal(OverlappingActors[0]->GetActorLocation(), GetActorLocation(), ForwardDirection->GetForwardVector());
		if (bCross)
		{
			// TODO: Teleport the objects too
			TeleportChar();
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

void APortalActor::TeleportChar()
{
	// Teleport the character.
	ACharacter* PC = UGameplayStatics::GetPlayerCharacter(World, 0);
	PC->SetActorLocationAndRotation(UpdateLocation(PC->GetActorLocation()), UpdateRotation(PC->GetActorRotation()));

	// Set the new control rotation.
	APlayerController* Cont = UGameplayStatics::GetPlayerController(World, 0);
	Cont->SetControlRotation(UpdateRotation(Cont->GetControlRotation()));

	// Update the velocity.
	PC->GetMovementComponent()->Velocity = UpdateVelocity(PC->GetMovementComponent()->Velocity);
	PC->GetMovementComponent()->UpdateComponentVelocity();

	// TODO: Smooth Orientation
	// Orient the player.
	FRotator InitialRot = Cont->GetControlRotation();
	Cont->SetControlRotation(FRotator(InitialRot.Pitch, InitialRot.Yaw, 0));
	PC->GetCapsuleComponent()->SetWorldRotation(FRotator::ZeroRotator);

	// Cut this frame
	UGameplayStatics::GetPlayerCameraManager(World, 0)->bGameCameraCutThisFrame = true;
}

