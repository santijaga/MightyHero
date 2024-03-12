


#include "UI/Widgets/ContinueUserWidgetBase.h"

#include "../../../MightyHeroGameModeBase.h"

void UContinueUserWidgetBase::Continue()
{
	if (AMightyHeroGameModeBase* GM = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM->StartGameplay(true);
	}
}

void UContinueUserWidgetBase::GameOver()
{
	if (AMightyHeroGameModeBase* GM = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM->GameOver(false);
	}
}