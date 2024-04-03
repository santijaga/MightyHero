


#include "Missions/Missions/MissionOne.h"

#include "Missions/MissionActor.h"

void AMissionOne::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[%s] %s"), *Name, *Description);
}