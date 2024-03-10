


#include "UI/UIController.h"

#include "UI/Widgets/MainWidgetBase.h"
#include "UI/Widgets/CollectionWidget.h"
#include "UI/Widgets/ContinueUserWidgetBase.h"
#include "UI/Widgets/GameplayWidgetBase.h"
#include "UI/Widgets/GameOverWidgetBase.h"

// Sets default values
AUIController::AUIController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUIController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Controller Interface
void AUIController::ShowMainMenuUI()
{
    HideAllWidgets();
    ShowMainWidget();
}

void AUIController::ShowGameplayUI()
{
    HideAllWidgets();
    ShowGameplayWidget();
}

void AUIController::ShowGameOverUI(bool bWithContinueTimeout)
{
    HideAllWidgets();
    ShowGameOverWidget();

    if (bWithContinueTimeout)
    {
        GetWorld()->GetTimerManager().SetTimer(EnableContinueTimerHandle, [this]()
            {
                EnableGameOverContinue();
            }, 2.0f, false);
    }
    else
    {
        EnableGameOverContinue();
    }
}

void AUIController::ShowCollectionUI(bool bRefreshOnly)
{
    if (!bRefreshOnly)
    {
        HideAllWidgets();
        ShowCollectionWidget();
    }

    RefreshCollection();
}

void AUIController::ShowContinueUI()
{
    HideAllWidgets();
    ShowContinueWidget();
}
// Controller Interface End

// MAIN WIDGET
void AUIController::ShowMainWidget()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (MainWidgetClass)
        {
            MainWidget = CreateWidget<UMainWidgetBase>(PC, MainWidgetClass);
            if (MainWidget)
            {
                MainWidget->ShowWidget();
            }
        }
    }
}

void AUIController::HideMainWidget()
{
    if (MainWidget)
    {
        MainWidget->RemoveWidget();
        MainWidget = nullptr;
    }
}

// GAMEPLAY WIDGET
void AUIController::ShowGameplayWidget()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (GameplayWidgetClass)
        {
            GameplayWidget = CreateWidget<UGameplayWidgetBase>(PC, GameplayWidgetClass);
            if (GameplayWidget)
            {
                GameplayWidget->ShowWidget();
            }
        }
    }
}

void AUIController::HideGameplayWidget()
{
    if (GameplayWidget)
    {
        GameplayWidget->RemoveWidget();
        GameplayWidget = nullptr;
    }
}

// GAMEOVER WIDGET
void AUIController::ShowGameOverWidget()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (GameOverWidgetClass)
        {
            GameOverWidget = CreateWidget<UGameOverWidgetBase>(PC, GameOverWidgetClass);
            if (GameOverWidget)
            {
                GameOverWidget->ShowWidget();
            }
        }
    }
}

void AUIController::HideGameOverWidget()
{
    if (GameOverWidget)
    {
        if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(EnableContinueTimerHandle))
        {
            GetWorld()->GetTimerManager().ClearTimer(EnableContinueTimerHandle);
        }

        GameOverWidget->RemoveWidget();
        GameOverWidget = nullptr;
    }
}

void AUIController::EnableGameOverContinue()
{
    if (GameOverWidget)
    {
        GameOverWidget->EnableContinue();
    }
}

void AUIController::OnEnableContinueTimerEnds()
{
    EnableGameOverContinue();
}

// COLLECTION WIDGET
void AUIController::ShowCollectionWidget()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (CollectionWidgetClass)
        {
            CollectionWidget = CreateWidget<UCollectionWidget>(PC, CollectionWidgetClass);
            if (CollectionWidget)
            {
                CollectionWidget->ShowWidget();
            }
        }
    }
}

void AUIController::HideCollectionWidget()
{
    if (CollectionWidget)
    {
        CollectionWidget->RemoveWidget();
        CollectionWidget = nullptr;
    }
}

void AUIController::RefreshCollection()
{
    if (CollectionWidget)
    {
        CollectionWidget->RefreshAllCards();
    }
}

// CONTINUE WIDGET
void AUIController::ShowContinueWidget()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (ContinueWidgetClass)
        {
            ContinueWidget = CreateWidget<UContinueUserWidgetBase>(PC, ContinueWidgetClass);
            if (ContinueWidget)
            {
                ContinueWidget->ShowWidget();
            }
        }
    }
}

void AUIController::HideContinueWidget()
{
    if (ContinueWidget)
    {
        ContinueWidget->RemoveWidget();
        ContinueWidget = nullptr;
    }
}

// UTILS
void AUIController::HideAllWidgets()
{
    HideMainWidget();
    HideGameplayWidget();
    HideGameOverWidget();
    HideCollectionWidget();
    HideContinueWidget();
}