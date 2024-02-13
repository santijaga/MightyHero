

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BackgroundController.generated.h"


UCLASS()
class MIGHTYHERO_API ABackgroundController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABackgroundController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	double BackgroundSegmentLength = 1600.0;
	double Threshold = 0;
	double DistanceLimit = 0;

private:
	UFUNCTION(BlueprintCallable, Category=Pawn)
	double GetCharacterHorizontalLocation();

	UFUNCTION(BlueprintCallable, Category=Background)
	void SpawnNextSegment();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Background)
	TSubclassOf<class ABackgroundActor> BackgroundActorClass;

private:
	TArray<ABackgroundActor*> Segments;

public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void ResetBackground();

private:
	UFUNCTION()
	void DestroyAllSegments();

	UFUNCTION()
	void InitStartSegments();
};
