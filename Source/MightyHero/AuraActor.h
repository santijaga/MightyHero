

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "AuraActor.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AAuraActor : public APaperFlipbookActor
{
	GENERATED_BODY()

	AAuraActor();
	
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float DefaultDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float ExpirationDuration = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Activate();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Deactivate(bool ShouldScheduleNextCollectable);

	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<class UPaperFlipbook> ExpireFlipbook;

private:
	UFUNCTION()
	void TrackCharacter();

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnExpire();

	UFUNCTION()
	void OnDurationEnds();

	FTimerHandle AuraDurationHandle;
};
