


#include "CollectionCardWidget.h"
#include "GameDataController.h"
#include "MightyHeroGameModeBase.h"

void UCollectionCardWidget::OnSelectItemFromCollection()
{
	if (UGameDataController* GameDataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		GameDataController->SaveActivePawnClass(CardValue);

		if (AMightyHeroGameModeBase* GameMode = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->RefreshPawn();
			GameMode->RefreshCollection();
		}
	}
}

void UCollectionCardWidget::RefreshCardStatus()
{
	if (UGameDataController* GameDataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		UClass* ActivePawnClass = GameDataController->LoadActivePawnClass();
		if (ActivePawnClass == CardValue)
		{
			isActive = true;
		}
		else
		{
			isActive = false;
		}
	}
}
