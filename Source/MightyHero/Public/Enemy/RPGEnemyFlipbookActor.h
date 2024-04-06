

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "RPGEnemyFlipbookActor.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ARPGEnemyFlipbookActor : public APaperFlipbookActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
