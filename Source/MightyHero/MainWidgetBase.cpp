#include "MainWidgetBase.h"
#include "GameDataController.h"
#include "MightyHeroGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"

int32 UMainWidgetBase::LoadHighScore()
{
	if (UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		return DataController->LoadHighScore();
	}

	return 0;
}

void UMainWidgetBase::StartGameplay()
{
	if (AMightyHeroGameModeBase* CurrentGameMode = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		CurrentGameMode->StartGameplay();
	}
}

void UMainWidgetBase::OnOpenCollection()
{
    if (AMightyHeroGameModeBase* CurrentGameMode = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        CurrentGameMode->HideMainWidget();
        CurrentGameMode->OpenCollection();
    }
}

void UMainWidgetBase::QuitGame()
{
    {
        UWorld* World = GetWorld();
        if (World)
        {
            APlayerController* PlayerController = World->GetFirstPlayerController();
            if (PlayerController)
            {
                UKismetSystemLibrary::QuitGame(
                    World,
                    PlayerController,
                    EQuitPreference::Quit,
                    false
                );
            }
        }
    }
}

int32 UMainWidgetBase::LoadCoins()
{
    if (UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
    {
        return DataController->LoadCoins();
    }

    return 0;
}
