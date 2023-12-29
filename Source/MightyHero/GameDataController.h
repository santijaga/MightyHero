#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameDataController.generated.h"

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
};
