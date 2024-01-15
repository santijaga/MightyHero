#include "GameOverWidgetBase.h"
#include "MightyHeroGameModeBase.h"
#include "GameDataController.h"
#include "MightyHeroPlayerState.h"
#include "MightyHeroPlayerController.h"

void UGameOverWidgetBase::BackToMainMenu()
{
	if (AMightyHeroGameModeBase* CurrentGameMode = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		CurrentGameMode->ResetGame();
	}
}

int32 UGameOverWidgetBase::LoadScores()
{
	if (UWorld* World = GetWorld())
	{
		if (AMightyHeroPlayerController* PlayerController = Cast<AMightyHeroPlayerController>(World->GetFirstPlayerController()))
		{
			if (AMightyHeroPlayerState* PlayerState = Cast<AMightyHeroPlayerState>(PlayerController->GetPawn()->GetPlayerState()))
			{
				return PlayerState->GetScores();
			}
		}
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

bool UGameOverWidgetBase::WasHighScore()
{
	return wasHighScore;
}

int32 UGameOverWidgetBase::GetScores()
{
	return CurrentScores;
}

void UGameOverWidgetBase::LoadData()
{
	CurrentScores = LoadScores();
	HighScores = LoadHighScores();

	wasHighScore = CurrentScores > HighScores;
}