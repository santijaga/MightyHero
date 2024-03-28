

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGMainMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API URPGMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category=ButtonBehaviour)
	void OnPlay();
};
