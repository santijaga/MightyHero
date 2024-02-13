#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "MainWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API UMainWidgetBase : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Data)
	int32 LoadHighScore();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void StartGameplay();

	UFUNCTION(BlueprintCallable, Category = UI)
	void OnOpenCollection();

	UFUNCTION(BlueprintCallable, Category = System)
	void QuitGame();

	UFUNCTION(BlueprintCallable, Category = Data)
	int32 LoadCoins();
};
