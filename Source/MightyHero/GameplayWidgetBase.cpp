#include "GameplayWidgetBase.h"
#include "MightyHeroPlayerController.h"
#include "MightyHeroPlayerState.h"
#include "Kismet/GameplayStatics.h"

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