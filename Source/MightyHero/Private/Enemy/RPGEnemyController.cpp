


#include "Enemy/RPGEnemyController.h"

#include "Core/MightyHeroRPGGameModeBase.h"
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
	if (AMightyHeroRPGGameModeBase* GM = Cast<AMightyHeroRPGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		float TopBound = GM->TopBound;
		float BotBound = GM->BottomBound;


	}
}