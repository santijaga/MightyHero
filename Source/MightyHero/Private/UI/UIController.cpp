


#include "UI/UIController.h"

#include "../MainWidgetBase.h"

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


// Main widget block start
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
    }
}