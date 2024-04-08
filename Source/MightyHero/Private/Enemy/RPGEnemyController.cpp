


#include "Enemy/RPGEnemyController.h"

#include "Core/MightyHeroRPGGameModeBase.h"
#include "Enemy/RPGEnemyFlipbookActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARPGEnemyController::ARPGEnemyController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARPGEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnMinorMaw();
}

// Called every frame
void ARPGEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
* Spawners
*/
void ARPGEnemyController::SpawnMinorMaw()
{
	SpawnEnemy(MinorMawClass);
}

/*
* Utils
*/
void ARPGEnemyController::SpawnEnemy(TSubclassOf<ARPGEnemyFlipbookActor> EnemyToSpawn) {
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Pawn)
	{
		float HorizontalDistance = Pawn->GetActorLocation().X + SpawnDistance;
		float VerticalPosition = FMath::RandRange(LowerBound, UpperBound);
		GetWorld()->SpawnActor<ARPGEnemyFlipbookActor>(EnemyToSpawn, FVector(HorizontalDistance, 0, VerticalPosition), FRotator(0, 0, 0));
	}
}