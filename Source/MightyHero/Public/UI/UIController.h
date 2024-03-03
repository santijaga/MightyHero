

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIController.generated.h"

class UMainWidgetBase;

/*
* UI Controller provide access to game widgets
*
* List of widgets:
* - Main widget
*/
UCLASS()
class MIGHTYHERO_API AUIController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Main widget block start

	/*
	* Main widget is main menu widget providing access to
	* - Starting gameplay
	* - Open collection
	* - Sound settings
	* - Quit game
	* 
	* Also this widget displays
	* - coins amount
	* - best scores
	*/
protected:
	UMainWidgetBase* MainWidget;
	
public:
	// Blueprint class for main widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UMainWidgetBase> MainWidgetClass;

	void ShowMainWidget();
	void HideMainWidget();

	// Main widget block end
};
