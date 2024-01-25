#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "SatelliteActor.generated.h"

class UPaperFlipbook;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ASatelliteActor : public APaperFlipbookActor
{
	ASatelliteActor();

	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Interaction)
	void Destruction();

	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> IdleFlipbook;

	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> DestructionFlipbook;

	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> SmokeFlipbook;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void DestroySatellite();

private:
	UFUNCTION()
	void SetupIdleFlipbook();

	UFUNCTION()
	void OnIdleFlipbookFinishedPlaying();

	UFUNCTION()
	void OnSmokeFlipbookFinishedPlaying();

private:
	bool bIsDestroyed = false;

	int32 repeatsTillSmoke = 1;

	UFUNCTION()
	void RemoveAllSubs(UPaperFlipbookComponent* RenderComponentRef);
};
