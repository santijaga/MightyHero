

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MightyHeroRPGGameModeBase.generated.h"

class ABackgroundController;
class ARPGEnemyController;
class ARPGUIController;
class ATrackCameraActorBase;

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
	* Rules
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rules)
	float BottomBound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rules)
	float TopBound;

	/*
	* Interface
	*/
public:
	UFUNCTION(BlueprintCallable, Category=Interface)
	void GameOver();

	UFUNCTION(BlueprintCallable, Category = Interface)
	void StartGame();

	/*
	* Spawns
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	TSubclassOf<ABackgroundController> BackgroundControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
	TSubclassOf<ARPGEnemyController> EnemyControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	TSubclassOf<ATrackCameraActorBase> TrackCameraClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	TSubclassOf<ARPGUIController> UIControllerClass;

	/*
	* Actors
	*/
private:
	ABackgroundController* BackgroundController;
	ATrackCameraActorBase* TrackCamera;
	ARPGEnemyController* EnemyController;
	ARPGUIController* UIController;

	/*
	* Accessors
	*/
public:
	UFUNCTION(BlueprintCallable, Category=Accessors)
	ABackgroundController* GetBackgroundController();

	ARPGEnemyController* GetEnemyController();

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
