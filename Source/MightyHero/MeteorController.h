

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeteorController.generated.h"

UCLASS()
class MIGHTYHERO_API AMeteorController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeteorController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	TSubclassOf<class AMeteorActor> MeteorActorClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float FirstSpawnDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float SpawnStepDistance = 500.f;

private:
	float CurrentSpawnStepDistance = 0;
	float NextSpawnDistance = 0;

	float SpawnThreshold = 1600.f;
	float ZThreshold = 600.f;

	float DefaultScale = 1;
	float CurrentScale = 1;

	void SpawnMeteor();

	UWorld* World;

public:
	UFUNCTION(Category = Gameplay)
	void DestroyAllMeteors(bool BlowerImpact);

	UFUNCTION(BlueprintCallable, Category = Tracking)
	bool HasAnyMeteorsOutOfBounds(double XBound, double ZBound);

public:
	UFUNCTION()
	void ResetController();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float minFallVelocity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float maxFallVelocity = 0;

private:
	UPROPERTY()
	int32 difficultyLevel = 1;

	UPROPERTY()
	float DefaultMinFallVelocity = 0;

	UPROPERTY()
	float DefaultMaxFallVelocity = 0;

public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void IncreaseDifficulty(int32 NewDifficulty);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	TArray<float> minFallVelocityPerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	TArray<float> maxFallVelocityPerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	TArray<float> scalePerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	TArray<float> spawnStepDistancePerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	TSubclassOf<class ASatelliteActor> SatelliteActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	int32 satelliteSpawnDifficultyLevel = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float defaultSatelliteSpawnChance = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float satelliteSpawnChanceIncreaceStep = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float satelliteSpawnMinHeight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float satelliteSpawnMaxHeight = 0;

private:
	UPROPERTY()
	float currentSatelliteSpawnChance = 0;

	UFUNCTION()
	bool ShouldSpawnSatellite();

	UFUNCTION()
	void SpawnSatellite();
};
