


#include "UI/RPGUIController.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widgets/RPGGameplayUserWidget.h"
#include "UI/Widgets/RPGMainMenuUserWidget.h"

// Sets default values
ARPGUIController::ARPGUIController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARPGUIController::BeginPlay()
{
	Super::BeginPlay();
	
	ShowMainMenuWidget();
}

// Called every frame
void ARPGUIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
* Interface
*/
void ARPGUIController::ShowGameplayUI()
{
	HideAllWidgets();
	ShowGameplayWidget();
}

void ARPGUIController::ShowMainMenuUI()
{
	HideAllWidgets();
	ShowMainMenuWidget();
}

/*
* Widget Control
*/
void ARPGUIController::HideGameplayWidget()
{
	if (GameplayWidget)
	{
		HideWidget(GameplayWidget);
	}
}

void ARPGUIController::HideMainMenuWidget()
{
	if (MainMenuWidget)
	{
		HideWidget(MainMenuWidget);
	}
}

void ARPGUIController::ShowGameplayWidget()
{
	if (GameplayWidgetClass)
	{
		ShowWidget(GameplayWidgetClass, GameplayWidget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Gameplay Widget Class not set."), *this->GetName());
	}
}

void ARPGUIController::ShowMainMenuWidget()
{
	if (MainMenuWidgetClass)
	{
		ShowWidget(MainMenuWidgetClass, MainMenuWidget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Main Menu Widget Class not set."), *this->GetName());
	}
}

/*
* Utils
*/
void ARPGUIController::HideAllWidgets()
{
	HideMainMenuWidget();
	HideGameplayWidget();
}

template<typename UserWidgetClass>
void ARPGUIController::HideWidget(UserWidgetClass* WidgetToClose)
{
	if (WidgetToClose)
	{
		Cast<UUserWidget>(WidgetToClose)->RemoveFromParent();
	}
}

template<typename UserWidgetClass, typename AssignType>
void ARPGUIController::ShowWidget(TSubclassOf<UserWidgetClass> WidgetClassToCreate, AssignType*& VariableToAssign)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (WidgetClassToCreate)
		{
			VariableToAssign = Cast<AssignType>(CreateWidget<UUserWidget>(PC, WidgetClassToCreate));
			if (VariableToAssign)
			{
				VariableToAssign->AddToViewport();
			}
		}
	}
}