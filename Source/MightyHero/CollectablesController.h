

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectablesController.generated.h"

class ABlower;
class ACharacterBase;
class ACollectableBase;
class AAuraCollectable;

UCLASS()
class MIGHTYHERO_API ACollectablesController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectablesController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SpawnBlower();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<ABlower> BlowerActorClass;

private:
	UPROPERTY()
	ACharacterBase* CharacterRef;

	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	float NextSpawnDistance = 0;

	UFUNCTION()
	bool ShouldSpawnBlower();

private:
	float CurrentSpawnStepDistance = 0;

	float SpawnThreshold = 1600.f;
	float ZThreshold = 600.f;

public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void StartGameplay();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void StopGameplay();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void DestroyAllCollectables();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void ResetController();

	UFUNCTION(BlueprintCallable, Category = Tracking)
	void DestroyAnyCollectablesOutOfBounds(double XBound, double ZBound);

private:
	UFUNCTION()
	void RunAllTimers();

	UFUNCTION()
	void ClearAllTimers();

private:
	UPROPERTY()
	int32 difficultyLevel = 1;

public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void IncreaseDifficulty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float SpawnStepDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float FirstSpawnDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float DefaultBlowerSpawnChance = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	int32 BlowerSpawnDifficultyLevel = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float BlowerSpawnChanceIncreaceStep = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	TArray<float> SpawnStepDistancePerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float BlowerSpawnMinHeight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float BlowerSpawnMaxHeight = 0;

private:
	UPROPERTY()
	float CurrentBlowerSpawnChance = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilitiesCollectable)
	bool IsAbilityCollectablesActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilitiesCollectable)
	TArray<TSubclassOf<ACollectableBase>> AbilitiesCollectables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilitiesCollectable)
	float AbilitiesCollectablesSpawnIndent = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilitiesCollectable)
	float AbilitiesCollectablesSpawnMinHeight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilitiesCollectable)
	float AbilitiesCollectablesMaxHeight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilitiesCollectable)
	float FirstAbilitiesCollectableSpawnTimer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilitiesCollectable)
	float AbilitiesCollectableSpawnTimer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilitiesCollectable)
	TSubclassOf<class AAuraActor> AuraActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilitiesCollectable)
	FVector AuraActorScale = FVector(1.f, 1.f, 1.f);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void ScheduleNextCollectable();

private:
	UFUNCTION()
	void SpawnFirstAbilitiesCollectable();

	UFUNCTION()
	void SpawnRandomAbilitiesCollectable();

	UFUNCTION()
	void SetupNextAbilitiesCollectableSpawn();

	UPROPERTY()
	FTimerHandle FirstAbilitiesCollectablesTimerHandle;

	UPROPERTY()
	FTimerHandle AbilitiesCollectablesTimerHandle;

public:
	UFUNCTION(BlueprintCallable, Category = Behaviour)
	void CollectableBehaviour(ACollectableBase* ActiveCollectable);

private:
	UFUNCTION()
	void BlowerBehaviour(ABlower* ActiveBlower);

	UFUNCTION()
	void AuraBehaviour(AAuraCollectable* ActiveAura);

};
