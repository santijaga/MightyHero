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
		UE_LOG(LogTemp, Warning, TEXT("Exists data in MightyHeroHighScore slot"));

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
		UE_LOG(LogTemp, Warning, TEXT("Exists data in MightyHeroSoundSettings slot"));

		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroSoundSettings"), 0)))
		{
			bSoundIsEnabledData = LoadGameInstance->bSoundIsEnabled;
			UE_LOG(LogTemp, Warning, TEXT("%s saved bSoundIsEnabled value"), bSoundIsEnabledData ? TEXT("true") : TEXT("false"));

			MainSlotInstance->bSoundIsEnabled = bSoundIsEnabledData;
			UGameplayStatics::DeleteGameInSlot(TEXT("MightyHeroSoundSettings"), 0);
			bWasSanitized = true;
		}
	}

	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroPawnSettings"), 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Exists data in MightyHeroPawnSettings slot"));

		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroPawnSettings"), 0)))
		{
			PawnClassData = LoadGameInstance->ActiveActorClass;
			UE_LOG(LogTemp, Warning, TEXT("%s saved pawned class"), *PawnClassData->GetName());
			MainSlotInstance->ActiveActorClass = PawnClassData;
			UGameplayStatics::DeleteGameInSlot(TEXT("MightyHeroPawnSettings"), 0);
			bWasSanitized = true;
		}
	}

	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroCoinsValue"), 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Exists data in MightyHeroCoinsValue slot"));

		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroCoinsValue"), 0)))
		{
			CoinsData = LoadGameInstance->Coins;
			UE_LOG(LogTemp, Warning, TEXT("%d saved Coins value"), CoinsData);
			UGameplayStatics::DeleteGameInSlot(TEXT("MightyHeroCoinsValue"), 0);
			MainSlotInstance->Coins = CoinsData;
			bWasSanitized = true;
		}
	}

	if (UGameplayStatics::DoesSaveGameExist(TEXT("MightyHeroUnlockedSkins"), 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Exists data in MightyHeroUnlockedSkins slot"));
		if (UMightyHeroSaveGame* LoadGameInstance = Cast<UMightyHeroSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MightyHeroUnlockedSkins"), 0)))
		{
			SkinsData = LoadGameInstance->UnlockedSkins;
			for (FString skin : SkinsData)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s skin unlocked"), *skin);
			}
			MainSlotInstance->UnlockedSkins = SkinsData;
			UGameplayStatics::DeleteGameInSlot(TEXT("MightyHeroUnlockedSkins"), 0);
			bWasSanitized = true;
		}
	}

	if (bWasSanitized)
	{
		SaveToMainSaveSlot(MainSlotInstance);
		
		MainSlotInstance = GetMainSaveSlot();
		UE_LOG(LogTemp, Warning, TEXT("%d saved HighScore value"), MainSlotInstance->HighScore);
		UE_LOG(LogTemp, Warning, TEXT("%s saved bSoundIsEnabled value"), MainSlotInstance->bSoundIsEnabled ? TEXT("true") : TEXT("false"));
		UE_LOG(LogTemp, Warning, TEXT("%s saved pawned class"), *MainSlotInstance->ActiveActorClass->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%d saved Coins value"), MainSlotInstance->Coins);
		for (FString skin : MainSlotInstance->UnlockedSkins)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s skin unlocked"), *skin);
		}
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