

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIController.generated.h"

class UMainWidgetBase;
class UCollectionWidget;
class UGameplayWidgetBase;
class UGameOverWidgetBase;

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

	/*
	* UI Controller Interface
	* 
	* Public methods to controll game UI
	* 
	* - ShowMainMenuUI
	* - ShowGameplayUI
	* - ShowGameoverUI
	* - ShowCollectionUI
	*/
public:
	void ShowMainMenuUI();
	void ShowGameplayUI();
	void ShowGameOverUI(bool bWithContinueTimeout);
	void ShowCollectionUI(bool bRefreshOnly);
	// Controller Interface End

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

protected:
	void ShowMainWidget();
	void HideMainWidget();

	// Main widget block end

	// Gameplay widget block start

	/*
	* Gameplay widget is in game widget providing access to
	* - Pause the game
	* - Quit back to main menu
	* - Toggle sound
	* 
	* This widget displays
	* - Current scores
	* - Current coins
	*/
protected:
	UGameplayWidgetBase* GameplayWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UGameplayWidgetBase> GameplayWidgetClass;

protected:
	void ShowGameplayWidget();
	void HideGameplayWidget();

	// Gameplay widget block end

	// Gameover widget block start
	/*
	* Gameover widget is in-game widget providing access to
	* - Return to menu
	* 
	* This widget displays
	* - Final scores
	* - Collected coins
	*/
protected:
	UGameOverWidgetBase* GameOverWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UGameOverWidgetBase> GameOverWidgetClass;

protected:
	void ShowGameOverWidget();
	void HideGameOverWidget();
	void EnableGameOverContinue();

private:
	FTimerHandle EnableContinueTimerHandle;
	void OnEnableContinueTimerEnds();
	// Gameover widget block over

	// Collection widget block start
	/*
	* Collection widget is widget providing access to
	* - Return to menu
	* 
	* This widget displays
	* - Collection cards
	*/
protected:
	UCollectionWidget* CollectionWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UCollectionWidget> CollectionWidgetClass;

protected:
	void ShowCollectionWidget();
	void HideCollectionWidget();
	void RefreshCollection();
	// Collection widget block over

	/*
	* Utils
	*
	* - Removing all widgets
	*/
private:
	void HideAllWidgets();
};