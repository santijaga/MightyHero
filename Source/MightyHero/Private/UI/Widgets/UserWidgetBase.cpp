#include "UI/Widgets/UserWidgetBase.h"
#include "Data/GameDataController.h"
#include "../../../MightyHeroGameModeBase.h"
#include "../../../SoundController.h"

void UUserWidgetBase::ShowWidget()
{
	this->AddToViewport();
}

void UUserWidgetBase::RemoveWidget()
{
	this->RemoveFromParent();
}

bool UUserWidgetBase::LoadSoundStatus()
{
	if (UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		return DataController->LoadSoundSetting();
	}

	return true;
}

void UUserWidgetBase::EnableSound()
{
	if (UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		DataController->SaveSoundSetting(true);
		if (ASoundController* SoundController = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetSoundController())
		{
			SoundController->SetSound(true);
		}
	}
}

void UUserWidgetBase::DisableSound()
{
	if (UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		DataController->SaveSoundSetting(false);
		if (ASoundController* SoundController = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetSoundController())
		{
			SoundController->SetSound(false);
		}
	}
}