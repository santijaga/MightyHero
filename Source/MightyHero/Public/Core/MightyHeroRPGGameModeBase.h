

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MightyHeroRPGGameModeBase.generated.h"

class ATrackCameraActorBase;
class ARPGUIController;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMightyHeroRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	/*
	* Spawns
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	TSubclassOf<ATrackCameraActorBase> TrackCameraClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	TSubclassOf<ARPGUIController> UIControllerClass;

	/*
	* Actors
	*/
private:
	ATrackCameraActorBase* TrackCamera;
	ARPGUIController* UIController;

	/*
	* Accessors
	*/
public:
	UFUNCTION(BlueprintCallable, Category=Accessors)
	ATrackCameraActorBase* GetTrackCamera();

	UFUNCTION(BlueprintCallable, Category=Accessors)
	ARPGUIController* GetUIController();

	/*
	* Utils
	*/
private:
	template <typename AssignType, typename SpawnClassType>
	void SpawnAndAssignActor(AssignType*& VariableToAssign, TSubclassOf<SpawnClassType> ClassToSpawn);
};
