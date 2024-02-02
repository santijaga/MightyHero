

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "CollectableBase.generated.h"

class UPaperFlipbook;
class UBoxComponent;
class ASoundController;
class USoundCue;
class ACharacterBase;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ACollectableBase : public APaperFlipbookActor
{
	GENERATED_BODY()

protected:
	ACollectableBase();

public:	
	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> IdleFlipbook;

	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> CollectedFlipbook;

	UFUNCTION(BlueprintCallable, Category = Interaction)
	void Collect();

private:
	UPROPERTY()
	bool bIsCollected = false;

public:
	UFUNCTION(BlueprintCallable, Category = Behaviour)
	void DestroyCollectable();

private:
	UFUNCTION()
	void SetupIdleFlipbook();
};
