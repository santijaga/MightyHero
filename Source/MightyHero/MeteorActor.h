#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "MeteorActor.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMeteorActor : public APaperFlipbookActor
{
	GENERATED_BODY()

	AMeteorActor();

protected:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category=Motion)
	void SetFallVelocity(double NewVelocity);

private:
	double FallVelocity;

public:
	UFUNCTION(BlueprintCallable, Category=Interaction)
	void Destruction();
};
