#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MightyHeroPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMightyHeroPlayerState : public APlayerState
{
	GENERATED_BODY()

	AMightyHeroPlayerState();

private:
	int32 Scores = 0;

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = State)
	void AddScore();

	UFUNCTION(BlueprintCallable, Category = State)
	int32 GetScores();
};
