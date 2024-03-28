


#include "Core/MightyHeroRPGGameModeBase.h"

#include "Camera/TrackCameraActorBase.h"
#include "UI/RPGUIController.h"

void AMightyHeroRPGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnAndAssignActor(TrackCamera, TrackCameraClass);
	SpawnAndAssignActor(UIController, UIControllerClass);
}

/*
* Accessors
*/
ATrackCameraActorBase* AMightyHeroRPGGameModeBase::GetTrackCamera()
{
	return TrackCamera;
}

ARPGUIController* AMightyHeroRPGGameModeBase::GetUIController()
{
	return UIController;
}

/*
* Utils
*/
template <typename AssignType, typename SpawnClassType>
void AMightyHeroRPGGameModeBase::SpawnAndAssignActor(AssignType*& VariableToAssign, TSubclassOf<SpawnClassType> ClassToSpawn)
{
	if (ClassToSpawn)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor<SpawnClassType>(ClassToSpawn);
		VariableToAssign = Cast<AssignType>(SpawnedActor);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Class not set."), *this->GetName());
	}
}