// Fill out your copyright notice in the Description page of Project Settings.


#include "MightyHeroGameModeBase.h"
#include "MightyHeroPlayerController.h"
#include "TrackCameraActorBase.h"
#include "EngineUtils.h"
#include "CharacterBase.h"
#include "MainWidgetBase.h"
#include "GameplayWidgetBase.h"
#include "MeteorController.h"

void AMightyHeroGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = Cast<AMightyHeroPlayerController>(GetWorld()->GetFirstPlayerController());

    if (PlayerController)
    {
        CharacterRef = Cast<ACharacterBase>(PlayerController->GetPawn());
    }

    InitUI();

    UWorld* World = GetWorld();
    if (World)
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = this;
        MeteorController = World->SpawnActor<AMeteorController>(MeteorControllerClass, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParameters);
    }
}

void AMightyHeroGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsGameOver)
    {
        TrackCharacterLocation();
        CheckForGameOver();
    }
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
    bIsCharacterFall = false;
}

void AMightyHeroGameModeBase::GameOver()
{
    UE_LOG(LogTemp, Warning, TEXT("Game is over"));

    bIsGameOver = true;
    MeteorController->DestroyAllMeteors();
    
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

void AMightyHeroGameModeBase::TrackCharacterLocation()
{
    UE_LOG(LogTemp, Warning, TEXT("Tracking character location:"))
    if (CharacterRef)
    {
        FVector CurrentCharacterLocation = CharacterRef->GetActorLocation();

        UE_LOG(LogTemp, Warning, TEXT("Character Z location: %f"), CurrentCharacterLocation.Z);

        if (CurrentCharacterLocation.Z < LowerBound)
        {
            CharacterRef->Fall();
            bIsCharacterFall = true;
        }
    }
}

void AMightyHeroGameModeBase::CheckForGameOver()
{
    if (bIsCharacterFall)
    {
        GameOver();
    }
}
