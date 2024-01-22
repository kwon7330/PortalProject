

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"


class APortalProjectCharacter;

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTALPROJECT_API IInteractable
{
	GENERATED_BODY()

	
public:
	virtual void Interact(APortalProjectCharacter* Character);
	virtual void ButtonInteract();
	virtual void FallingBall();
};
