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
	bool bIsGameover = true;
	int32 Coins = 0;
	int32 CurrentHitPoints;
	int32 CurrentShieldPoints;
	int32 MaxHitPoints;
	int32 MaxShieldPoints;
	int32 Scores = 0;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	/*
	* Interface
	*/
public:
	UFUNCTION(BlueprintCallable, Category = State)
	void AddCoin();

	UFUNCTION(BlueprintCallable, Category = State)
	void AddScore();

	void ApplyDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category = State)
	int32 GetCoins();
	
	int32 GetCurrentHitPoints();
	int32 GetCurrentShieldPoints();
	int32 GetMaxHitPoints();
	int32 GetMaxShieldPoints();

	UFUNCTION(BlueprintCallable, Category = State)
	int32 GetScores();

	UFUNCTION(BlueprintCallable, Category = State)
	void ResetCoins();

	UFUNCTION(BlueprintCallable, Category = State)
	void ResetScores();

	UFUNCTION(BlueprintCallable, Category = State)
	void StartGameplay();

	/*
	* Utils
	*/
private:
	int CalculateDistanceScores();
};
