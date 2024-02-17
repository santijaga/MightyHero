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

void UGameDataController::SaveActivePawnClass(UClass* SelectedActorClass)
{
	UMightyHeroSaveGame* SaveGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::CreateSaveGameObject(UMightyHeroSaveGame::StaticClass()));
	SaveGameInstance->ActiveActorClass = SelectedActorClass;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MightyHeroPawnSettings"), 0);
}

UClass* UGameDataController::LoadActivePawnClass()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroPawnSettings"), 0))
	{
		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroPawnSettings"), 0)))
		{
			return LoadGameInstance->ActiveActorClass;
		}
	}

	return nullptr;
}

void UGameDataController::SaveCoins(int32 Coins)
{
	UMightyHeroSaveGame* SaveGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::CreateSaveGameObject(UMightyHeroSaveGame::StaticClass()));
	SaveGameInstance->Coins = Coins;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MightyHeroCoinsValue"), 0);
}

int32 UGameDataController::LoadCoins()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroCoinsValue"), 0))
	{
		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroCoinsValue"), 0)))
		{
			return LoadGameInstance->Coins;
		}
	}

	return 0;
}

void UGameDataController::SaveUnlockedSkins(TArray<FString> Skins)
{
	UMightyHeroSaveGame* SaveGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::CreateSaveGameObject(UMightyHeroSaveGame::StaticClass()));
	SaveGameInstance->UnlockedSkins = Skins;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MightyHeroUnlockedSkins"), 0);
}

TArray<FString> UGameDataController::LoadUnlockedSkins()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroUnlockedSkins"), 0))
	{
		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroUnlockedSkins"), 0)))
		{
			return LoadGameInstance->UnlockedSkins;
		}
	}

	return TArray<FString>();
}
