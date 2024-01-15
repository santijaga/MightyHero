

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	TSubclassOf<class AMeteorActor> MeteorActorClass;

private:
	class ACharacterBase* CharacterRef;

	float FirstSpawnDistance = 0;
	float SpawnStepDistance = 0;
	float NextSpawnDistance = 0;

	float SpawnThreshold = 1600.f;
	float ZThreshold = 600.f;

	void SpawnMeteor();

	UWorld* World;

public:
	UFUNCTION(Category = Gameplay)
	void DestroyAllMeteors();

	UFUNCTION(BlueprintCallable, Category = Tracking)
	bool HasAnyMeteorsOutOfBounds(double XBound, double ZBound);

public:
	UFUNCTION()
	void ResetController();
};
