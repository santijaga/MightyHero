


#include "Missions/Missions/MissionOne.h"

#include "Core/MightyHeroRPGGameModeBase.h"
#include "Enemy/RPGEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "Missions/MissionActor.h"

void AMissionOne::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[%s] %s"), *Name, *Description);
}

void AMissionOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsStarted)
	{
		bIsCompleted = IsCompleted();
	}
}

/*
* Interface
*/
void AMissionOne::StartGameplay()
{
	Super::StartGameplay();

	GetWorld()->GetTimerManager().SetTimer(StartMissionHandle, this, &AMissionOne::StartMission, StartMissionDelay, false);
}

/*
* Behaviour
*/
bool AMissionOne::IsCompleted()
{
	if (MinorMawsToDestroy == MinorMawsDestroyed)
	{
		return true;
	}

	return false;
}

void AMissionOne::SetupEnemiesSpawning()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnMinorMawsTimerHandle, this, &AMissionOne::SpawnMinorMaw, NextMawSpawnDelay, true, FirstMawSpawnDelay);
}

void AMissionOne::StartMission()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Mission started"), *this->GetName());
	bIsStarted = true;
	SetupEnemiesSpawning();
}

void AMissionOne::SpawnMinorMaw()
{
	if (AMightyHeroRPGGameModeBase* GM = Cast<AMightyHeroRPGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (ARPGEnemyController* EC = GM->GetEnemyController())
		{
			EC->SpawnMinorMaw();
		}
	}
}