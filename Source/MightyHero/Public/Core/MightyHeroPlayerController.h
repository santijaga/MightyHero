

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MightyHeroPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMightyHeroPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
