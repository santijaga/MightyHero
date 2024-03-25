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
	int32 Coins = 0;
	bool bIsGameover = true;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
public:
	UFUNCTION(BlueprintCallable, Category = State)
	void AddScore();

	UFUNCTION(BlueprintCallable, Category = State)
	int32 GetScores();

	UFUNCTION(BlueprintCallable, Category = State)
	void ResetScores();

	UFUNCTION(BlueprintCallable, Category = State)
	void AddCoin();

	UFUNCTION(BlueprintCallable, Category = State)
	int32 GetCoins();

	UFUNCTION(BlueprintCallable, Category = State)
	void ResetCoins();
};
