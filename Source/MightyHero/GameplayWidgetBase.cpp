#include "GameplayWidgetBase.h"
#include "MightyHeroPlayerController.h"
#include "MightyHeroPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "SoundController.h"
#include "MightyHeroGameModeBase.h"

int32 UGameplayWidgetBase::GetScores()
{
	if (AMightyHeroPlayerController* PlayerController = Cast<AMightyHeroPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (AMightyHeroPlayerState* PlayerState = Cast<AMightyHeroPlayerState>(PlayerController->PlayerState))
		{
			return PlayerState->GetScores();
		}
	}

	return 0;
}

int32 UGameplayWidgetBase::GetCoins()
{
	if (AMightyHeroPlayerController* PlayerController = Cast<AMightyHeroPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (AMightyHeroPlayerState* PlayerState = Cast<AMightyHeroPlayerState>(PlayerController->PlayerState))
		{
			return PlayerState->GetCoins();
		}
	}

	return 0;
}

void UGameplayWidgetBase::TogglePause()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (!isResuming)
		{
			secondsToResume = 3;
			isResuming = true;
		}
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void UGameplayWidgetBase::ResumeGame()
{
	isResuming = false;
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UGameplayWidgetBase::BackToMainMenu()
{
	if (UWorld* World = GetWorld())
	{
		if (ASoundController* SoundController = Cast<ASoundController>((Cast<AMightyHeroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSoundController())))
		{
			SoundController->PlayShortCollectCue();
		}

		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(World, true);
		UGameplayStatics::OpenLevel(World, FName(*CurrentLevelName), false);
	}
}
