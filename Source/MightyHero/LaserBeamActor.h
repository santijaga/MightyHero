

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "LaserBeamActor.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ALaserBeamActor : public APaperFlipbookActor
{
	GENERATED_BODY()
public:
	ALaserBeamActor();

	virtual void BeginPlay() override;

	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<class UPaperFlipbook> IdleFlipbook;

private:
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void DestroyLaser();
};
