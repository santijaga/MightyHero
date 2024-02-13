


#include "CoinsController.h"
#include "Coin.h"

// Sets default values
ACoinsController::ACoinsController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACoinsController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoinsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoinsController::SpawnCoin(FVector InSpawnLocation)
{
	if (GetWorld())
	{
		if (CoinClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			FRotator SpawnRotation = FRotator(0, 0, 0);

			ACoin* SpawnedCoin = GetWorld()->SpawnActor<ACoin>(CoinClass, InSpawnLocation, SpawnRotation, SpawnParams);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[ACoinsController] Coin Class is not setup!"));
		}
	}
}

