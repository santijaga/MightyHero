#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameDataController.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API UGameDataController : public UObject
{
	GENERATED_BODY()
	
public:
	void SaveHighScore(int32 Score);
	int32 LoadHighScore();
	void SaveSoundSetting(bool bNewSoundValue);
	bool LoadSoundSetting();
	void SaveActivePawnClass(UClass* SelectedActorClass);
	UClass* LoadActivePawnClass();
	void SaveCoins(int32 Coins);
	int32 LoadCoins();
	void SaveUnlockedSkins(TArray<FString> Skins);
	TArray<FString> LoadUnlockedSkins();
};
