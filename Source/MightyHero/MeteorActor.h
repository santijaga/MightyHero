#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "MeteorActor.generated.h"

class UPaperFlipbook;

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
	void SetFallVelocity(float NewVelocity);

private:
	double FallVelocity;

public:
	UFUNCTION(BlueprintCallable, Category = Interaction)
	void Destruction();
	void Destruction(bool bShouldSpawnCoin);

	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> DestructionFlipbook;

private:
	UFUNCTION()
	void OnFlipbookFinishedPlaying();

private:
	bool bIsDestroyed = false;
};
