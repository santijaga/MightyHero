

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoinsController.generated.h"

UCLASS()
class MIGHTYHERO_API ACoinsController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoinsController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	TSubclassOf<class ACoin> CoinClass;

	UFUNCTION(BlueprintCallable, Category = Spawn)
	void SpawnCoin(FVector InSpawnLocation);
};
