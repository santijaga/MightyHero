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

	UFUNCTION(BlueprintCallable, Category = Data)
	bool LoadSoundStatus();

	UFUNCTION(BlueprintCallable, Category = Sound)
	void EnableSound();

	UFUNCTION(BlueprintCallable, Category = Sound)
	void DisableSound();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void StartGameplay();
};
