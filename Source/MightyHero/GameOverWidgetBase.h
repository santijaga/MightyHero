

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "GameOverWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API UGameOverWidgetBase : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void BackToMainMenu();

	UFUNCTION(BlueprintCallable, Category = Data)
	bool WasHighScore();

	UFUNCTION(BlueprintCallable, Category = Data)
	int32 GetScores();

	UFUNCTION(BlueprintCallable, Category = Data)
	int32 GetCollectedCoins();

	UFUNCTION(BlueprintCallable, Category = Data)
	void LoadData();

	UFUNCTION()
	void EnableContinue();

private:
	bool wasHighScore = false;
	bool bCanContinue = false;
	int32 HighScores = 0;
	int32 CurrentScores = 0;
	int32 CollectedCoins = 0;

	int32 LoadScores();
	int32 LoadHighScores();
	int32 LoadCollectedCoins();
};
