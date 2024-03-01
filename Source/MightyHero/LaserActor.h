

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "LaserActor.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ALaserActor : public APaperFlipbookActor
{
	GENERATED_BODY()
	
	ALaserActor();

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float DefaultDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float ExpirationDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float HorizontalOffset = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float VerticalOffset = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<class ALaserBeamActor> LaserBeamActorClass;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Activate();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Deactivate(bool ShouldScheduleNextCollectable);

	UPROPERTY(Category = Sprite, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<class UPaperFlipbook> ExpireFlipbook;

private:
	bool bIsActivated = false;
	bool bIsOnCooldown = false;

	void TrackCharacter();
	void OnExpire();
	void OnDurationEnds();
	void TrackMeteors();
	void ResetCooldown();

	FTimerHandle LaserDurationHandle;
	FTimerHandle CooldownTimerHandle;
};
