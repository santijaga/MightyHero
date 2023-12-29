// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MightyHeroGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMightyHeroGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable, Category = Camera)
	void InitCameras();

protected:
	UFUNCTION(BlueprintCallable)
	virtual	void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	ACameraActor* MainCamera;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	class UMainWidgetBase* MainWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UMainWidgetBase> MainWidgetClass;

	UFUNCTION(BlueprintCallable, Category = UI)
	void InitUI();

	UFUNCTION(BlueprintCallable, Category = UI)
	void ShowMainWidget();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void StartGameplay();

	UFUNCTION(BlueprintCallable, Category = UI)
	void HideMainWidget();
};
