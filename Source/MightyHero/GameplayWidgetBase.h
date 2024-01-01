#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "GameplayWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API UGameplayWidgetBase : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Text)
	int32 GetScores();
};
