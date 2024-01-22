// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"

#include "Components/Image.h"
#include "PortalProject/PortalProjectCharacter.h"

void UPlayerUI::AddedPortal(EPortalType Portal)
{
	bool bIsLeft = true;
	
	switch (Portal)
	{
	case EPortalType::Player1Blue:
		break;
	case EPortalType::Player1Purple:
		bIsLeft = false;
		break;
	case EPortalType::Player2Orange:
		break;
	case EPortalType::Player2Red:
		bIsLeft = false;
		break;
	}
	if (bIsLeft)
	{
		LeftPortalIndicator->SetBrushFromTexture(LeftFull);
		LeftPortalIndicator->SetBrushTintColor(*PortalColorMap.Find(Portal));
	}
	else
	{
		RightPortalIndicator->SetBrushFromTexture(RightFull);
		RightPortalIndicator->SetBrushTintColor(*PortalColorMap.Find(Portal));
	}
}

void UPlayerUI::RemovedPortal(EPortalType Portal)
{
	bool bIsLeft = true;
	
	switch (Portal)
	{
	case EPortalType::Player1Blue:
		break;
	case EPortalType::Player1Purple:
		bIsLeft = false;
		break;
	case EPortalType::Player2Orange:
		break;
	case EPortalType::Player2Red:
		bIsLeft = false;
		break;
	}

	if (bIsLeft)
	{
		LeftPortalIndicator->SetBrushFromTexture(LeftBlank);
		LeftPortalIndicator->SetBrushTintColor(FLinearColor::White);
	}
	else
	{
		RightPortalIndicator->SetBrushFromTexture(RightBlank);
		RightPortalIndicator->SetBrushTintColor(FLinearColor::White);
	}
}

void UPlayerUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	RemovedPortal(EPortalType::Player1Blue);
	RemovedPortal(EPortalType::Player1Purple);
}
