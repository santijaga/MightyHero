#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MightyHeroSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API UMightyHeroSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 HighScore;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	bool bSoundIsEnabled;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	UClass* ActiveActorClass;

	UPROPERTY(VisibleAnywhere, Category = Currency)
	int32 Coins;

	UPROPERTY(VisibleAnywhere, Category = Skins)
	TArray<FString> UnlockedSkins;
};
