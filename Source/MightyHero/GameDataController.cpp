#include "GameDataController.h"
#include "MightyHeroSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UGameDataController::SaveHighScore(int32 Score)
{
	UMightyHeroSaveGame* SaveGameInstance = GetMainSaveSlot();
	SaveGameInstance->HighScore = Score;
	SaveToMainSaveSlot(SaveGameInstance);
}

int32 UGameDataController::LoadHighScore()
{
	if (UMightyHeroSaveGame* LoadGameInstance = GetMainSaveSlot())
	{
		return LoadGameInstance->HighScore;
	}
	
	return 0;
}

void UGameDataController::SaveSoundSetting(bool bNewSoundValue)
{
	UMightyHeroSaveGame* SaveGameInstance = GetMainSaveSlot();
	SaveGameInstance->bSoundIsEnabled = bNewSoundValue;
	SaveToMainSaveSlot(SaveGameInstance);
}

bool UGameDataController::LoadSoundSetting()
{
	if (UMightyHeroSaveGame* LoadGameInstance = GetMainSaveSlot())
	{
		return LoadGameInstance->bSoundIsEnabled;
	}

	return true;
}

void UGameDataController::SaveActivePawnClass(UClass* SelectedActorClass)
{
	UMightyHeroSaveGame* SaveGameInstance = GetMainSaveSlot();
	SaveGameInstance->ActiveActorClass = SelectedActorClass;
	SaveToMainSaveSlot(SaveGameInstance);
}

UClass* UGameDataController::LoadActivePawnClass()
{
	if (UMightyHeroSaveGame* LoadGameInstance = GetMainSaveSlot())
	{
		return LoadGameInstance->ActiveActorClass;
	}

	return nullptr;
}

void UGameDataController::SaveCoins(int32 Coins)
{
	UMightyHeroSaveGame* SaveGameInstance = GetMainSaveSlot();
	SaveGameInstance->Coins = Coins;
	SaveToMainSaveSlot(SaveGameInstance);
}

int32 UGameDataController::LoadCoins()
{
	if (UMightyHeroSaveGame* LoadGameInstance = GetMainSaveSlot())
	{
		return LoadGameInstance->Coins;
	}

	return 0;
}

void UGameDataController::SaveUnlockedSkins(TArray<FString> Skins)
{
	UMightyHeroSaveGame* SaveGameInstance = GetMainSaveSlot();
	SaveGameInstance->UnlockedSkins = Skins;
	SaveToMainSaveSlot(SaveGameInstance);
}

TArray<FString> UGameDataController::LoadUnlockedSkins()
{
	if (UMightyHeroSaveGame* LoadGameInstance = GetMainSaveSlot())
	{
		return LoadGameInstance->UnlockedSkins;
	}

	return TArray<FString>();
}

void UGameDataController::SanitizeSaveData()
{
	int32 HighScoreData;
	bool bSoundIsEnabledData;
	UClass* PawnClassData;
	int32 CoinsData;
	TArray<FString> SkinsData;
	bool bWasSanitized = false;

	UMightyHeroSaveGame* MainSlotInstance = GetMainSaveSlot();

	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroHighScore"), 0))
	{
		if (UMightyHeroSaveGame* HighScoreGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroHighScore"), 0)))
		{
			HighScoreData = HighScoreGameInstance->HighScore;
			MainSlotInstance->HighScore = HighScoreData;
			UGameplayStatics::DeleteGameInSlot(TEXT("MightyHeroHighScore"), 0);
			bWasSanitized = true;
		}
	}

	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroSoundSettings"), 0))
	{
		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroSoundSettings"), 0)))
		{
			bSoundIsEnabledData = LoadGameInstance->bSoundIsEnabled;
			MainSlotInstance->bSoundIsEnabled = bSoundIsEnabledData;
			UGameplayStatics::DeleteGameInSlot(TEXT("MightyHeroSoundSettings"), 0);
			bWasSanitized = true;
		}
	}

	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroPawnSettings"), 0))
	{
		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroPawnSettings"), 0)))
		{
			PawnClassData = LoadGameInstance->ActiveActorClass;
			MainSlotInstance->ActiveActorClass = PawnClassData;
			UGameplayStatics::DeleteGameInSlot(TEXT("MightyHeroPawnSettings"), 0);
			bWasSanitized = true;
		}
	}

	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroCoinsValue"), 0))
	{
		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroCoinsValue"), 0)))
		{
			CoinsData = LoadGameInstance->Coins;
			UGameplayStatics::DeleteGameInSlot(TEXT("MightyHeroCoinsValue"), 0);
			MainSlotInstance->Coins = CoinsData;
			bWasSanitized = true;
		}
	}

	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroUnlockedSkins"), 0))
	{
		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroUnlockedSkins"), 0)))
		{
			SkinsData = LoadGameInstance->UnlockedSkins;
			MainSlotInstance->UnlockedSkins = SkinsData;
			UGameplayStatics::DeleteGameInSlot(TEXT("MightyHeroUnlockedSkins"), 0);
			bWasSanitized = true;
		}
	}

	if (bWasSanitized)
	{
		SaveToMainSaveSlot(MainSlotInstance);
	}

}

UMightyHeroSaveGame* UGameDataController::GetMainSaveSlot()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroMainSlot"), 0))
	{
		return Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroMainSlot"), 0));
	}

	return Cast<UMightyHeroSaveGame>(UGameplayStatics::CreateSaveGameObject(UMightyHeroSaveGame::StaticClass()));
}

void UGameDataController::SaveToMainSaveSlot(UMightyHeroSaveGame* SaveGameInstance)
{
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MightyHeroMainSlot"), 0);
}