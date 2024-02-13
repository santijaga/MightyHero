#pragma once

#include "CoreMinimal.h"
#include "CollectableBase.h"
#include "Coin.generated.h"

class UProjectileMovementComponent;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ACoin : public ACollectableBase
{
	GENERATED_BODY()

public:
	ACoin();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Collect() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

private:
	bool bIsAcceleratingTowardsPlayer = false;
	FVector TargetDirection;
	float AccelerationSpeed = 0.f;
	float MaxSpeed = 3000.f;
	float AccelerationRate = 1500.f;
	float DecelerationRate = 2000.f;
	float MinSpeedBeforeAccelerating = 100.f;

	void ControlCoinMovement(float DeltaTime);

	void SlowDownCoin(float DeltaTime);
	void SpeedUpCoin(float DeltaTime);
};
