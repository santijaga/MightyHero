


#include "UI/Widgets/CollectionWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"

#include "UI/UIController.h"
#include "UI/Widgets/CollectionCardWidget.h"
#include "../../../MightyHeroGameModeBase.h"
#include "../../../GameDataController.h"
#include "../../../SoundController.h"

void UCollectionWidget::CloseCollection()
{
	if (AMightyHeroGameModeBase* GameMode = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
        if (ASoundController* SoundController = Cast<ASoundController>(GameMode->GetSoundController()))
        {
            SoundController->PlayShortCollectCue();
        }

        if (AUIController* UIController = GameMode->GetUIController())
        {
            UIController->ShowMainMenuUI();
        }
	}

	this->RemoveWidget();
}

void UCollectionWidget::RefreshAllCards()
{
    if (this->WidgetTree != nullptr)
    {
        TArray<UWidget*> AllWidgets;
        this->WidgetTree->GetAllWidgets(AllWidgets);
        for (UWidget* Widget : AllWidgets)
        {
            // Attempt to cast the widget to the child widget class
            UCollectionCardWidget* CardWidget = Cast<UCollectionCardWidget>(Widget);
            if (CardWidget != nullptr)
            {
                // Call the method on the child widget
                CardWidget->RefreshCardStatus();
            }
        }
    }
}

void UCollectionWidget::LockAllCards()
{
	if (UGameDataController* GameDataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
	{
		TArray<FString> UnlockedSkins = GameDataController->LoadUnlockedSkins();

		UnlockedSkins.Empty();

		GameDataController->SaveUnlockedSkins(UnlockedSkins);

        RefreshAllCards();
	}
}

void UCollectionWidget::AddMoreCoins()
{
    if (UGameDataController* GameDataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass()))
    {
        int32 AvailibleCoins = GameDataController->LoadCoins();

        GameDataController->SaveCoins(AvailibleCoins + 100);
    }
}

