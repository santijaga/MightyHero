#include "MainWidgetBase.h"
#include "GameDataController.h"
#include "MightyHeroGameModeBase.h"

int32 UMainWidgetBase::LoadHighScore()
{
	if (UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		return DataController->LoadHighScore();
	}

	return 0;
}

bool UMainWidgetBase::LoadSoundStatus()
{
	if (UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		return DataController->LoadSoundSetting();
	}

	return true;
}

void UMainWidgetBase::EnableSound()
{
	if (UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		DataController->SaveSoundSetting(true);
	}
}

void UMainWidgetBase::DisableSound()
{
	if (UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		DataController->SaveSoundSetting(false);
	}
}

void UMainWidgetBase::StartGameplay()
{
	if (AMightyHeroGameModeBase* CurrentGameMode = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		CurrentGameMode->StartGameplay();
	}
}