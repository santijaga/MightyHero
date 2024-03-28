


#include "UI/Widgets/RPGMainMenuUserWidget.h"

#include "Core/MightyHeroRPGGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/RPGPawn.h"
#include "UI/RPGUIController.h"

void URPGMainMenuUserWidget::OnPlay()
{
	if (AMightyHeroRPGGameModeBase* GM = Cast<AMightyHeroRPGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (ARPGUIController* UIController = Cast<ARPGUIController>(GM->GetUIController()))
		{
			UIController->ShowGameplayUI();
		}
	}

	if (ARPGPawn* Pawn = Cast<ARPGPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		Pawn->StartGameplay();
	}
}