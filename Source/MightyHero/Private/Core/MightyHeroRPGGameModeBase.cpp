


#include "Core/MightyHeroRPGGameModeBase.h"

#include "Background/BackgroundController.h"
#include "Camera/TrackCameraActorBase.h"
#include "Core/MightyHeroPlayerState.h"
#include "Enemy/RPGEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "Missions/MissionsController.h"
#include "Pawn/RPGPawn.h"
#include "UI/RPGUIController.h"

void AMightyHeroRPGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnAndAssignActor(BackgroundController, BackgroundControllerClass);
	SpawnAndAssignActor(EnemyController, EnemyControllerClass);
	SpawnAndAssignActor(MissionsController, MissionsControllerClass);
	SpawnAndAssignActor(TrackCamera, TrackCameraClass);
	SpawnAndAssignActor(UIController, UIControllerClass);
}

/*
* Interface
*/
void AMightyHeroRPGGameModeBase::GameOver()
{
	if (ARPGPawn* Pawn = Cast<ARPGPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		Pawn->GameOver();
	}

	if (UIController)
	{
		UIController->GameOver();
	}
}

void AMightyHeroRPGGameModeBase::StartGame()
{
	if (ARPGPawn* Pawn = Cast<ARPGPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		Pawn->StartGame();
	}

	if (UIController)
	{
		UIController->StartGame();
	}

	if (AMightyHeroPlayerState* PS = Cast<AMightyHeroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
	{
		PS->StartGameplay();
	}

	if (MissionsController)
	{
		MissionsController->StartGameplay();
	}
}

/*
* Accessors
*/
ABackgroundController* AMightyHeroRPGGameModeBase::GetBackgroundController()
{
	return BackgroundController;
}

ARPGEnemyController* AMightyHeroRPGGameModeBase::GetEnemyController()
{
	return EnemyController;
}

AMissionsController* AMightyHeroRPGGameModeBase::GetMissionsController()
{
	return MissionsController;
}

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