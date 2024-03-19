

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "ContinueUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API UContinueUserWidgetBase : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category=Gameplay)
	void Continue();

	UFUNCTION(BlueprintCallable, Category=Gameplay)
	void EnableContinue();

	UFUNCTION(BlueprintCallable, Category=Gameplay)
	void GameOver();

	UFUNCTION(BlueprintCallable, Category=Gameplay)
	bool IsADLoaded();

	UFUNCTION(BlueprintCallable, Category=Gameplay)
	bool IsContinueEnabled();

	UFUNCTION(BlueprintCallable, Category=Gameplay)
	void MarkADAsLoaded();

private:
	bool bIsContinueEnabled = false;
	bool bIsADLoaded = false;
};
