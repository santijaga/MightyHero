


#include "UI/Widgets/CollectionCardWidget.h"
#include "UI/UIController.h"
#include "../GameDataController.h"
#include "../MightyHeroGameModeBase.h"

void UCollectionCardWidget::OnSelectItemFromCollection()
{
	if (bIsUnlockedByDefault || !bIsLocked)
	{
		if (UGameDataController* GameDataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
		{
			GameDataController->SaveActivePawnClass(CardValue);

			if (AMightyHeroGameModeBase* GameMode = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
			{
				GameMode->RefreshPawn();

				if (AUIController* UIController = GameMode->GetUIController())
				{
					UIController->ShowCollectionUI(true);
				}
			}
		}
	}
	else
	{
		bShouldShowUnlockDialog = true;
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

		TArray<FString> UnlockedSkins = GameDataController->LoadUnlockedSkins();

		if (UnlockedSkins.IsValidIndex(UnlockedSkins.Find(CardName)))
		{
			bIsLocked = false;
		}
		else
		{
			bIsLocked = true;
		}

		bShouldShowConfirmDialog = false;
		bShouldShowFailDialog = false;
		bShouldShowUnlockDialog = false;
	}
}



bool UCollectionCardWidget::GetIsCardLocked()
{
	if (bIsUnlockedByDefault)
	{
		return false;
	}

	return bIsLocked;
}

void UCollectionCardWidget::OnUnlockButtonPressed()
{
	if (UGameDataController* GameDataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		int32 AvailibleCoins = GameDataController->LoadCoins();

		bShouldShowUnlockDialog = false;

		if (AvailibleCoins < UnlockCost)
		{
			bShouldShowFailDialog = true;
		}
		else
		{
			bShouldShowConfirmDialog = true;
		}
	}
}

void UCollectionCardWidget::OnFailureButtonPressed()
{
	bShouldShowFailDialog = false;
}

void UCollectionCardWidget::OnConfirmButtonPressed()
{
	if (UGameDataController* GameDataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		TArray<FString> UnlockedSkins = GameDataController->LoadUnlockedSkins();

		UnlockedSkins.Add(CardName);

		GameDataController->SaveUnlockedSkins(UnlockedSkins);

		int32 AvailibleCoins = GameDataController->LoadCoins();

		GameDataController->SaveCoins(AvailibleCoins - UnlockCost);

		if (AMightyHeroGameModeBase* GameMode = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			if (AUIController* UIController = GameMode->GetUIController())
			{
				UIController->ShowCollectionUI(true);
			}
		}
	}
}

void UCollectionCardWidget::OnCancelButtonPressed()
{
	RefreshCardStatus();
}
