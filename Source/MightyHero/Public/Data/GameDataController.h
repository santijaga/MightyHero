#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameDataController.generated.h"

class ACharacterBase;
class UMightyHeroSaveGame;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API UGameDataController : public UObject
{
	GENERATED_BODY()
	
public:
	UClass* LoadActivePawnClass();
	int32 LoadCoins();
	int32 LoadEnergy();
	int32 LoadHighScore();
	bool LoadSoundSetting();
	int32 LoadVitality();
	int32 LoadStrength();
	TArray<FString> LoadUnlockedSkins();
	void SanitizeSaveData();
	void SaveActivePawnClass(UClass* SelectedActorClass);
	void SaveCoins(int32 Coins);
	void SaveEnergy(int32 NewEnergy);
	void SaveHighScore(int32 Score);
	void SaveSoundSetting(bool bNewSoundValue);
	void SaveVitality(int32 NewVitality);
	void SaveStrength(int32 NewStrength);
	void SaveUnlockedSkins(TArray<FString> Skins);

private:
	UMightyHeroSaveGame* GetMainSaveSlot();
	void SaveToMainSaveSlot(UMightyHeroSaveGame* SaveGameInstance);
};
