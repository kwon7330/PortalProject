


#include "PlayerMove.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PortalProject/PortalProjectCharacter.h"


UPlayerMove::UPlayerMove()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	
}



void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	Self = Cast<APortalProjectCharacter>(GetOwner());
	if (APlayerController* PlayerController = Cast<APlayerController>(Self->Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC, 0);
		}
	}
	
	
}



void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
}

void UPlayerMove::SetupPlayerInput(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &UPlayerMove::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &UPlayerMove::StopJump);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UPlayerMove::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UPlayerMove::Look);
	}
}


void UPlayerMove::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Self->Controller != nullptr)
	{
		// add movement 
		Self->AddMovementInput(Self->GetActorForwardVector(), MovementVector.Y);
		Self->AddMovementInput(Self->GetActorRightVector(), MovementVector.X);
		//UE_LOG(LogTemp,Warning,TEXT("MoveMoveMove!!!!!!!!!!!!!!!!!!"))
	}
}
void UPlayerMove::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Self->Controller != nullptr)
	{
		// add yaw and pitch input to controller
		Self->AddControllerYawInput(LookAxisVector.X);
		Self->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void UPlayerMove::Jump()
{
	Self->Jump();
}

void UPlayerMove::StopJump()
{
	Self->StopJumping();
}

void UPlayerMove::ServerRPC_Move_Implementation()
{
	
}

void UPlayerMove::FootStepSound()
{
	
	float RandomNum = FMath::FRand();
	FVector SpawnSound = Self->GetMesh()->GetComponentLocation();
	if(RandomNum<0.3f)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),MoveSound1,SpawnSound);
	}
	else UGameplayStatics::PlaySoundAtLocation(GetWorld(),MoveSound2,SpawnSound);

	
}



void UPlayerMove::MultiRPC_Move_Implementation()
{
	FootStepSound();
}

