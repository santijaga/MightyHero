

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

private:
	UFUNCTION()
	int32 LoadScores();

	UFUNCTION()
	int32 LoadHighScores();

	bool wasHighScore = false;
	int32 HighScores = 0;
	int32 CurrentScores = 0;

public:
	UFUNCTION(BlueprintCallable, Category = Data)
	bool WasHighScore();

	UFUNCTION(BlueprintCallable, Category = Data)
	int32 GetScores();

	UFUNCTION(BlueprintCallable, Category = Data)
	void LoadData();

private:
	UPROPERTY()
	bool bCanContinue = false;

public:
	UFUNCTION(Category = State)
	void EnableContinue();
};
