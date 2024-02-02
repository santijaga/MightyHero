// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MightyHeroGameModeBase.generated.h"

class UGameDataController;
class ABackgroundController;
class ASoundController;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMightyHeroGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable, Category = Camera)
	void InitCameras();

	class ACharacterBase* CharacterRef;
	class AMightyHeroPlayerController* PlayerController;

	bool bIsGameOver = true;

protected:
	UFUNCTION(BlueprintCallable)
	virtual	void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class ATrackCameraActorBase* MainCamera;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	class UMainWidgetBase* MainWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UMainWidgetBase> MainWidgetClass;

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

	UPROPERTY(BlueprintReadWrite, Category = UI)
	class UGameplayWidgetBase* GameplayWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UGameplayWidgetBase> GameplayWidgetClass;

	UFUNCTION(BlueprintCallable, Category = UI)
	void ShowGameplayWidget();

	UFUNCTION(BlueprintCallable, Category = UI)
	void HideGameplayWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rules)
	double LowerBound = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rules)
	double BackBound = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rules)
	double UpperBound = 0;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void GameOver();

private:
	bool bIsCharacterFall = false;

	UFUNCTION(BlueprintCallable, Category = Tracking)
	FVector TrackCharacterLocation();

	void CheckForGameOver();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controllers)
	TSubclassOf<class AMeteorController> MeteorControllerClass;

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
	class UGameOverWidgetBase* GameOverWidget;

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
	void ResetScores();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Controllers)
	TSubclassOf<ABackgroundController> BackgroundControllerClass;

private:
	ABackgroundController* BackgroundController;

	UFUNCTION()
	void ResetBackground();

private:
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
	TSubclassOf<class ACollectablesController> CollectablesControllerClass;

private:
	ACollectablesController* CollectablesController;

public:
	UFUNCTION(BlueprintCallable, Category = Access)
	AMeteorController* GetMeteorController();

	UFUNCTION(BlueprintCallable, Category = Access)
	ASoundController* GetSoundController();

	UFUNCTION(BlueprintCallable, Category = Access)
	ACollectablesController* GetCollectablesController();
};
