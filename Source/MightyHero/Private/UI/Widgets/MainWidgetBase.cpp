#include "UI/Widgets/MainWidgetBase.h"
#include "Data/GameDataController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundController.h"
#include "UI/UIController.h"
#include "../../../MightyHeroGameModeBase.h"


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
	if (AMightyHeroGameModeBase* CurrentGameMode = Cast<AMightyHeroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		CurrentGameMode->StartGameplay(false);
	}
}

void UMainWidgetBase::OnOpenCollection()
{
    if (AMightyHeroGameModeBase* CurrentGameMode = Cast<AMightyHeroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        if (AUIController* UIController = CurrentGameMode->GetUIController())
        {
            UIController->ShowCollectionUI(false);

            if (ASoundController* SoundController = Cast<ASoundController>((CurrentGameMode->GetSoundController())))
            {
                SoundController->PlayShortCollectCue();
            }
        }

    }
}

void UMainWidgetBase::QuitGame()
{
    {
        UWorld* World = GetWorld();
        if (World)
        {
            APlayerController* PlayerController = World->GetFirstPlayerController();

            if (ASoundController* SoundController = Cast<ASoundController>((Cast<AMightyHeroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSoundController())))
            {
                SoundController->PlayShortCollectCue();
            }

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
