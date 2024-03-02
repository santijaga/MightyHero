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
class UCollectionWidget;
class UGameDataController;
class UGameplayWidgetBase;
class UGameOverWidgetBase;
class UMainWidgetBase;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMightyHeroGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	bool bIsGameOver = true;
	bool bIsCharacterFall = false;
	
	AMightyHeroPlayerController* PlayerController;

	virtual	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = Camera)
	void InitCameras();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Tracking)
	FVector TrackCharacterLocation();

	void CheckForGameOver();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rules)
	double LowerBound = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rules)
	double BackBound = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rules)
	double UpperBound = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	ATrackCameraActorBase* MainCamera;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	UMainWidgetBase* MainWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UMainWidgetBase> MainWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	UGameplayWidgetBase* GameplayWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UGameplayWidgetBase> GameplayWidgetClass;
	
	UFUNCTION(BlueprintCallable, Category = UI)
	void InitUI();

	UFUNCTION(BlueprintCallable, Category = UI)
	void ShowMainWidget();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void StartGameplay();

	UFUNCTION(BlueprintCallable, Category = UI)
	void HideMainWidget();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	bool IsGameOver();

	UFUNCTION(BlueprintCallable, Category = UI)
	void ShowGameplayWidget();

	UFUNCTION(BlueprintCallable, Category = UI)
	void HideGameplayWidget();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void GameOver();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controllers)
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

	UPROPERTY(BlueprintReadWrite, Category = UI)
	UGameOverWidgetBase* GameOverWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UGameOverWidgetBase> GameOverWidgetClass;

	UFUNCTION(BlueprintCallable, Category = UI)
	void ShowGameOverWidget();

	UFUNCTION(BlueprintCallable, Category = UI)
	void HideGameOverWidget();

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

private:
	FTimerHandle EnableContinueTimerHandle;

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

private:
	UPROPERTY()
	UCollectionWidget* CollectionWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UCollectionWidget> CollectionWidgetClass;

	UFUNCTION(BlueprintCallable, Category = UI)
	void OpenCollection();

	UFUNCTION(BlueprintCallable, Category = Pawn)
	void RefreshPawn();

	UFUNCTION(BlueprintCallable, Category = UI)
	void RefreshCollection();

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
