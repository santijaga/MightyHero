

#pragma once

#include "CoreMinimal.h"
#include "Missions/MissionActor.h"
#include "MissionOne.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMissionOne : public AMissionActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
	* Interface
	*/
public:
	virtual void StartGameplay() override;

	/*
	* State
	*/
private:
	bool bIsCompleted = false;
	bool bIsStarted = false;
	int32 MinorMawsDestroyed = 0;

	/*
	* Behaviour
	*/
private:
	bool IsCompleted();
	void SetupEnemiesSpawning();
	void StartMission();
	void SpawnMinorMaw();

	/*
	* Rules
	*/
private:
	float StartMissionDelay = 3.0f;
	int32 MinorMawsToDestroy = 30;
	float FirstMawSpawnDelay = 3.0f;
	float NextMawSpawnDelay = 2.0f;

	/*
	* Handles
	*/
private:
	FTimerHandle StartMissionHandle;
	FTimerHandle SpawnMinorMawsTimerHandle;
};
