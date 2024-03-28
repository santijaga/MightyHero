

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

	/*
	* SETTINGS
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	FVector InitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	FVector Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	FRotator OffsetRotation;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
private:
	void TrackCharacter();	
};
