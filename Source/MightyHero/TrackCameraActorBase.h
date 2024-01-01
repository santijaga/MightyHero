

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "TrackCameraActorBase.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ATrackCameraActorBase : public ACameraActor
{
	GENERATED_BODY()

	ATrackCameraActorBase();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
private:
	class ACharacterBase* CharacterRef;

	double CharacterCurrentXPosition = .0f;

	double GetCharacterCurrentXLocation();

public:
	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void TrackCharacter();
};
