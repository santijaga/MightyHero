

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = UI)
	void ShowWidget();

	UFUNCTION(BlueprintCallable, Category = UI)
	void RemoveWidget();

	UFUNCTION(BlueprintCallable, Category = Data)
	bool LoadSoundStatus();

	UFUNCTION(BlueprintCallable, Category = Sound)
	void EnableSound();

	UFUNCTION(BlueprintCallable, Category = Sound)
	void DisableSound();
};
