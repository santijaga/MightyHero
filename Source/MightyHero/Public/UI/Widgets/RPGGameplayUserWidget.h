

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGGameplayUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API URPGGameplayUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/*
	* Interface
	*/
public:
	UFUNCTION(BlueprintCallable, Category=Data)
	float GetHealthPercentage();

	UFUNCTION(BlueprintCallable, Category=Mission)
	FString GetMissionDescription();

	UFUNCTION(BlueprintCallable, Category = Mission)
	FString GetMissionName();

	UFUNCTION(BlueprintCallable, Category=Data)
	int32 GetScores();

	UFUNCTION(BlueprintCallable, Category=Data)
	float GetShieldPercentage();
};
