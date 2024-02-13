


#include "CollectionWidget.h"
#include "MightyHeroGameModeBase.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "CollectionCardWidget.h"

void UCollectionWidget::CloseCollection()
{
	if (AMightyHeroGameModeBase* GameMode = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->ShowMainWidget();
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

