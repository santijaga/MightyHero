


#include "Core/MightyHeroPlayerController.h"

#include "Data/GameDataController.h"

void AMightyHeroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bEnableClickEvents = true;

	LoadCharacteristics();
}

int32 AMightyHeroPlayerController::GetEnergy()
{
	return Energy;
}

int32 AMightyHeroPlayerController::GetVitality()
{
	return Vitality;
}

int32 AMightyHeroPlayerController::GetStrength()
{
	return Strength;
}

void AMightyHeroPlayerController::LoadCharacteristics()
{
	if (UGameDataController* DC = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		Energy = DC->LoadEnergy();
		Vitality = DC->LoadVitality();
		Strength = DC->LoadStrength();
	}
}