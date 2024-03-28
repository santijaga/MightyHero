

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGUIController.generated.h"

class URPGMainMenuUserWidget;
class URPGGameplayUserWidget;

UCLASS()
class MIGHTYHERO_API ARPGUIController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGUIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	* Interface
	*/
public:
	UFUNCTION(BlueprintCallable, Category = UI)
	void ShowGameplayUI();

	UFUNCTION(BlueprintCallable, Category=UI)
	void ShowMainMenuUI();

	/*
	* Widget Classes
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
	TSubclassOf<URPGGameplayUserWidget> GameplayWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets)
	TSubclassOf<URPGMainMenuUserWidget> MainMenuWidgetClass;

	/*
	* Widget Actors
	*/
private:
	URPGGameplayUserWidget* GameplayWidget;
	URPGMainMenuUserWidget* MainMenuWidget;

	/*
	* Widget Control
	*/
private:
	void HideGameplayWidget();
	void HideMainMenuWidget();
	void ShowGameplayWidget();
	void ShowMainMenuWidget();

	/*
	* Utils
	*/
private:
	void HideAllWidgets();

	template<typename UserWidgetClass>
	void HideWidget(UserWidgetClass* WidgetToClose);

	template<typename UserWidgetClass, typename AssignType>
	void ShowWidget(TSubclassOf<UserWidgetClass> WidgetClassToCreate, AssignType*& VariableToAssign);
};
