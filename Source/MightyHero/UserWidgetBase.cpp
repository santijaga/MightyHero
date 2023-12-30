#include "UserWidgetBase.h"
#include "GameDataController.h"

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
	}
}

void UUserWidgetBase::DisableSound()
{
	if (UGameDataController* DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		DataController->SaveSoundSetting(false);
	}
}