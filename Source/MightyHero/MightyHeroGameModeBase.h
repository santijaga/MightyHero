// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MightyHeroGameModeBase.generated.h"

class ABackgroundController;
class ACoinsController;
class ACollectablesController;
class AMeteorController;
class AMightyHeroPlayerController;
class ASoundController;
class ATrackCameraActorBase;
class UGameDataController;
class AUIController;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMightyHeroGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	/*
	* Controllers
	*/
protected:
	void SetupControllers();

	template<typename ControllerClass>
	void SetupController(TSubclassOf<ControllerClass>& ControllerClassRef, ControllerClass*& ControllerInstance);

	// UIController
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controllers)
	TSubclassOf<AUIController> UIControllerClass;

	AUIController* GetUIController();

protected:
	AUIController* UIController;

	// End of Controllers


protected:
	bool bIsGameOver = true;
	bool bWasGameContinued = false;
	bool bIsCharacterFall = false;
	FTimerHandle ContinueTimerHandle;
	void OnContinue();

	virtual	void BeginPlay() override;
	void CheckForGameOver();

	UFUNCTION(BlueprintCallable, Category = Camera)
	void InitCameras();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Tracking)
	FVector TrackCharacterLocation();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rules)
	double LowerBound = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rules)
	double BackBound = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rules)
	double UpperBound = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	ATrackCameraActorBase* MainCamera;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void StartGameplay(bool bIsContinue);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	bool IsGameOver();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void GameOver(bool bFirstTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Controllers)
	TSubclassOf<AMeteorController> MeteorControllerClass;

private:
	AMeteorController* MeteorController;

	UFUNCTION()
	bool CheckForMeteorsOutOfBounds();

	UFUNCTION()
	void CheckForCollectablesOutOfBounds();

public:
	UFUNCTION()
	void ResetGame();

private:
	UPROPERTY()
	AActor* MainPlayerStart;

	UFUNCTION()
	void ResetCharacter();

	UGameDataController* DataController;

	UFUNCTION()
	void SaveHighScore();

	UFUNCTION()
	void SaveCoins();

	UFUNCTION()
	void ResetScores();

	UFUNCTION()
	void ResetCoins();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controllers)
	TSubclassOf<ABackgroundController> BackgroundControllerClass;

private:
	ABackgroundController* BackgroundController;

	UFUNCTION()
	void ResetBackground();

private:
	int32 DifficultyLevel = 0;

	UFUNCTION()
	void IncreaseDifficulty();

	UPROPERTY()
	int32 nextIncreaseScores = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	int32 difficultyStep = 5;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controllers)
	TSubclassOf<ASoundController> SoundControllerClass;

private:
	ASoundController* SoundController;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controllers)
	TSubclassOf<ACollectablesController> CollectablesControllerClass;

private:
	ACollectablesController* CollectablesController;

public:
	UFUNCTION(BlueprintCallable, Category = Access)
	AMeteorController* GetMeteorController();

	UFUNCTION(BlueprintCallable, Category = Access)
	ASoundController* GetSoundController();

	UFUNCTION(BlueprintCallable, Category = Access)
	ACollectablesController* GetCollectablesController();

	UFUNCTION(BlueprintCallable, Category = Pawn)
	void RefreshPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controllers)
	TSubclassOf<ACoinsController> CoinsControllerClass;

private:
	ACoinsController* CoinsController;

public:
	UFUNCTION(BlueprintCallable, Category = Controllers)
	ACoinsController* GetCoinsController();

	UFUNCTION(BlueprintCallable, Category = State)
	int32 GetCurrentDifficulty();
};
