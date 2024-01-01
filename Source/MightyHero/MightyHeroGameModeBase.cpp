// Fill out your copyright notice in the Description page of Project Settings.


#include "MightyHeroGameModeBase.h"
#include "MightyHeroPlayerController.h"
#include "TrackCameraActorBase.h"
#include "EngineUtils.h"
#include "CharacterBase.h"
#include "MainWidgetBase.h"
#include "GameplayWidgetBase.h"

void AMightyHeroGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = Cast<AMightyHeroPlayerController>(GetWorld()->GetFirstPlayerController());

    if (PlayerController)
    {
        CharacterRef = Cast<ACharacterBase>(PlayerController->GetPawn());
    }

    InitUI();
}

void AMightyHeroGameModeBase::InitUI()
{
    ShowMainWidget();
}

void AMightyHeroGameModeBase::StartGameplay()
{
    HideMainWidget();
    ShowGameplayWidget();

    if (CharacterRef)
    {
        CharacterRef->StartGameplay();
    }

    bIsGameOver = false;
}

void AMightyHeroGameModeBase::HideMainWidget()
{
    if (MainWidget)
    {
        MainWidget->RemoveWidget();
    }
}

void AMightyHeroGameModeBase::InitCameras()
{
    for (TActorIterator<ATrackCameraActorBase> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        ATrackCameraActorBase* Camera = *ActorItr;

        if (Camera->ActorHasTag(FName("Camera")))
        {
            if (Camera->ActorHasTag(FName("Main")))
            {
                MainCamera = Camera;
            }
        }
    }
}

void AMightyHeroGameModeBase::ShowMainWidget()
{
    if (PlayerController)
    {
        if (MainWidgetClass)
        {
            MainWidget = Cast<UMainWidgetBase>(CreateWidget<UUserWidget>(PlayerController, MainWidgetClass));
            if (MainWidget)
            {
                MainWidget->ShowWidget();
            }
        }
    }
}

bool AMightyHeroGameModeBase::IsGameOver()
{
    return bIsGameOver;
}

void AMightyHeroGameModeBase::ShowGameplayWidget()
{
    if (PlayerController)
    {
        if (GameplayWidgetClass)
        {
            GameplayWidget = Cast<UGameplayWidgetBase>(CreateWidget<UUserWidget>(PlayerController, GameplayWidgetClass));
            if (GameplayWidget)
            {
                GameplayWidget->ShowWidget();
            }
        }
    }
}

void AMightyHeroGameModeBase::HideGameplayWidget()
{
    if (GameplayWidget)
    {
        GameplayWidget->RemoveWidget();
    }
}