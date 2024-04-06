

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGEnemyController.generated.h"

class ARPGEnemyFlipbookActor;

UCLASS()
class MIGHTYHERO_API ARPGEnemyController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGEnemyController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	* Settings
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float LowerBound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	float SpawnDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	float UpperBound;

	/*
	* Enemy Classes
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	TSubclassOf<ARPGEnemyFlipbookActor> MinorMawClass;

	/*
	* Spawners
	*/
public:
	UFUNCTION(BlueprintCallable, Category=Spawner)
	void SpawnMinorMaw();

	/*
	* Utils
	*/
private:
	void SpawnEnemy(TSubclassOf<ARPGEnemyFlipbookActor> EnemyToSpawn);
};
