


#include "Missions/MissionActor.h"

// Sets default values
AMissionActor::AMissionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMissionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMissionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
* Interface
*/

FString AMissionActor::GetDescription()
{
	return Description;
}

FString AMissionActor::GetName()
{
	return Name;
}

void AMissionActor::StartGameplay()
{
}