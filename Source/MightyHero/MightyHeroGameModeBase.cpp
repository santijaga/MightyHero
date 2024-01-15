// Fill out your copyright notice in the Description page of Project Settings.


#include "MightyHeroGameModeBase.h"
#include "MightyHeroPlayerController.h"
#include "TrackCameraActorBase.h"
#include "EngineUtils.h"
#include "CharacterBase.h"
#include "MainWidgetBase.h"
#include "GameplayWidgetBase.h"
#include "MeteorController.h"
#include "GameOverWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameDataController.h"
#include "MightyHeroPlayerState.h"

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

    if (PlayerController)
    {
        FString PlayerStartTag = "MainStart";
        MainPlayerStart = FindPlayerStart(PlayerController, PlayerStartTag);
    }

    DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass());
}

void AMightyHeroGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsGameOver)
    {
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

    if (MeteorController)
    {
        MeteorController->ResetController();
    }

    bIsGameOver = false;
    bIsCharacterFall = false;
}

void AMightyHeroGameModeBase::GameOver()
{
    UE_LOG(LogTemp, Warning, TEXT("Game is over"));

    bIsGameOver = true;
    MeteorController->DestroyAllMeteors();
    HideGameplayWidget();
    ShowGameOverWidget();
    SaveHighScore();
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

FVector AMightyHeroGameModeBase::TrackCharacterLocation()
{
    if (CharacterRef)
    {
        FVector CurrentCharacterLocation = CharacterRef->GetActorLocation();

        return CurrentCharacterLocation;
    }

    return FVector(0, 0, 0);
}

void AMightyHeroGameModeBase::CheckForGameOver()
{
    FVector CurrentCharacterLocation = TrackCharacterLocation();

    if (CharacterRef && CurrentCharacterLocation.Z < LowerBound || CurrentCharacterLocation.Z > UpperBound)
    {
        CharacterRef->Fall();
        bIsCharacterFall = true;
    }

    if (bIsCharacterFall || CheckForMeteorsOutOfBounds())
    {
        CharacterRef->EarthDestroyed();
        GameOver();
    }
}

bool AMightyHeroGameModeBase::CheckForMeteorsOutOfBounds()
{
    bool isAnyMeteorsOutOfBounds = false;

    if (MeteorController)
    {
        double XBound = MainCamera->GetActorLocation().X + BackBound;
        isAnyMeteorsOutOfBounds = MeteorController->HasAnyMeteorsOutOfBounds(XBound, LowerBound);
    }

    return isAnyMeteorsOutOfBounds;
}

void AMightyHeroGameModeBase::ResetGame()
{
    HideGameOverWidget();
    ShowMainWidget();
    ResetCharacter();
    ResetScores();
}

void AMightyHeroGameModeBase::ShowGameOverWidget()
{
    if (PlayerController)
    {
        if (GameOverWidgetClass)
        {
            GameOverWidget = Cast<UGameOverWidgetBase>(CreateWidget<UUserWidget>(PlayerController, GameOverWidgetClass));
            if (GameOverWidget)
            {
                GameOverWidget->ShowWidget();
            }
        }
    }
}

void AMightyHeroGameModeBase::HideGameOverWidget()
{
    if (GameOverWidget)
    {
        GameOverWidget->RemoveWidget();
    }
}

void AMightyHeroGameModeBase::ResetCharacter()
{
    if (CharacterRef)
    {
        CharacterRef->SetActorLocation(MainPlayerStart->GetActorLocation());
        CharacterRef->ResetCharacter();
    }
}

void AMightyHeroGameModeBase::SaveHighScore()
{
    if (CharacterRef)
    {
        int32 Scores = CharacterRef->GetPlayerState<AMightyHeroPlayerState>()->GetScores();
        int32 HighScores = DataController->LoadHighScore();

        if (Scores > HighScores)
        {
            DataController->SaveHighScore(Scores);
        }
    }
}

void AMightyHeroGameModeBase::ResetScores()
{
    if (CharacterRef)
    {
        CharacterRef->GetPlayerState<AMightyHeroPlayerState>()->ResetScores();
    }
}
