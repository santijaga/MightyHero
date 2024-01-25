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

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void TogglePause();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	bool isResuming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	int32 secondsToResume = 0;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void ResumeGame();
};
