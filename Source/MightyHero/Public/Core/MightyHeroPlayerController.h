

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MightyHeroPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMightyHeroPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	/*
	* Interface
	*/
public:
	int32 GetEnergy();

	UFUNCTION(BlueprintCallable, Category=Characteristics)
	int32 GetVitality();
	int32 GetStrength();

	UFUNCTION(BlueprintCallable, Category=Characteristics)
	void LoadCharacteristics();

	/*
	* Characterisctics
	*/
private:
	int32 Energy;
	int32 Vitality;
	int32 Strength;
};
