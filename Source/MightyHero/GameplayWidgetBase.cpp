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