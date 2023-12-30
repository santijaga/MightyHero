// Fill out your copyright notice in the Description page of Project Settings.


#include "MightyHeroGameModeBase.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"
#include "MainWidgetBase.h"
#include "CharacterBase.h"

void AMightyHeroGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    CharacterRef = Cast<ACharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void AMightyHeroGameModeBase::InitUI()
{
    ShowMainWidget();
}

void AMightyHeroGameModeBase::StartGameplay()
{
    HideMainWidget();

    if (CharacterRef)
    {
        CharacterRef->StartGameplay();
    }
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
    for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        ACameraActor* Camera = *ActorItr;

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
    if (MainWidgetClass)
    {
        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            MainWidget = Cast<UMainWidgetBase>(CreateWidget<UUserWidget>(PlayerController, MainWidgetClass));
            if (MainWidget)
            {
                MainWidget->ShowWidget();
            }
        }
    }
}