#include "GameOverWidgetBase.h"
#include "MightyHeroGameModeBase.h"
#include "GameDataController.h"
#include "MightyHeroPlayerState.h"
#include "MightyHeroPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidgetBase::BackToMainMenu()
{
	if (bCanContinue)
	{
		if (AMightyHeroGameModeBase* CurrentGameMode = Cast<AMightyHeroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			CurrentGameMode->ResetGame();
		}
	}
}

int32 UGameOverWidgetBase::LoadScores()
{
	if (AMightyHeroPlayerState* PlayerState = Cast<AMightyHeroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
	{
		return PlayerState->GetScores();
	}

	return 0;
}

int32 UGameOverWidgetBase::LoadHighScores()
{
	if (UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		return DataController->LoadHighScore();
	}

	return 0;
}

int32 UGameOverWidgetBase::LoadCollectedCoins()
{
	if (AMightyHeroPlayerState* PlayerState = Cast<AMightyHeroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
	{
		return PlayerState->GetCoins();
	}

	return 0;
}

bool UGameOverWidgetBase::WasHighScore()
{
	return wasHighScore;
}

int32 UGameOverWidgetBase::GetScores()
{
	return CurrentScores;
}

int32 UGameOverWidgetBase::GetCollectedCoins()
{
	return CollectedCoins;
}

void UGameOverWidgetBase::LoadData()
{
	CurrentScores = LoadScores();
	HighScores = LoadHighScores();
	wasHighScore = CurrentScores > HighScores;
	CollectedCoins = LoadCollectedCoins();
}

void UGameOverWidgetBase::EnableContinue()
{
	bCanContinue = true;
}