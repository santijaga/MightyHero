


#include "UI/UIController.h"

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

