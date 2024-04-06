


#include "Missions/MissionsController.h"

#include "Data/GameDataController.h"
#include "Missions/MissionActor.h"

// Sets default values
AMissionsController::AMissionsController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMissionsController::BeginPlay()
{
	Super::BeginPlay();
	
	UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass());
	int32 CompletedMission = DataController->LoadMission();
	if (CompletedMission != 0)
	{
		NextMission = CompletedMission + 1;
	}

	if (MissionsList.IsValidIndex(NextMission))
	{
		CurrentMission = GetWorld()->SpawnActor<AMissionActor>(MissionsList[NextMission]);
	}
}

// Called every frame
void AMissionsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
* Interface
*/
FString AMissionsController::GetMissionDescription()
{
	return CurrentMission->GetDescription();
}

FString AMissionsController::GetMissionName()
{
	return CurrentMission->GetName();
}

void AMissionsController::StartGameplay()
{
	CurrentMission->StartGameplay();
}