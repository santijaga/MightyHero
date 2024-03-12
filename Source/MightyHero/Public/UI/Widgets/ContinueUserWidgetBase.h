

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
	void GameOver();
};
