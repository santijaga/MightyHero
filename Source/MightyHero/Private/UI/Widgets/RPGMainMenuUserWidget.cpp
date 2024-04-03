


#include "UI/Widgets/RPGMainMenuUserWidget.h"

#include "Core/MightyHeroRPGGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void URPGMainMenuUserWidget::OnPlay()
{
	if (AMightyHeroRPGGameModeBase* GM = Cast<AMightyHeroRPGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GM->StartGame();
	}
}