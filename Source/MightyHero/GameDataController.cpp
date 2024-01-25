#include "GameDataController.h"
#include "MightyHeroSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UGameDataController::SaveHighScore(int32 Score)
{
	UMightyHeroSaveGame* SaveGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::CreateSaveGameObject(UMightyHeroSaveGame::StaticClass()));
	SaveGameInstance->HighScore = Score;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MightyHeroHighScore"), 0);
}

int32 UGameDataController::LoadHighScore()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroHighScore"), 0))
	{
		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroHighScore"), 0)))
		{
			return LoadGameInstance->HighScore;
		}
	}
	
	return 0;
}

void UGameDataController::SaveSoundSetting(bool bNewSoundValue)
{
	UMightyHeroSaveGame* SaveGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::CreateSaveGameObject(UMightyHeroSaveGame::StaticClass()));
	SaveGameInstance->bSoundIsEnabled = bNewSoundValue;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MightyHeroSoundSettings"), 0);
}

bool UGameDataController::LoadSoundSetting()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroSoundSettings"), 0))
	{
		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroSoundSettings"), 0)))
		{
			return LoadGameInstance->bSoundIsEnabled;
		}
	}

	return true;
}
