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
#include "BackgroundController.h"
#include "SoundController.h"

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

    if (World)
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = this;
        BackgroundController = World->SpawnActor<ABackgroundController>(BackgroundControllerClass, FVector(0, -100.f, 0), FRotator(0, 0, 0), SpawnParameters);
    }

    if (World && SoundControllerClass)
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = this;
        SoundController = World->SpawnActor<ASoundController>(SoundControllerClass, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParameters);

        if (SoundController)
        {
            SoundController->SetSound(DataController->LoadSoundSetting());
        }
    }
}

void AMightyHeroGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsGameOver)
    {
        CheckForGameOver();
        IncreaseDifficulty();
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

    nextIncreaseScores = difficultyStep;
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

    bool bCharacterCrashedInSatellite = false;
    if (CharacterRef && CharacterRef->bCrashedInSatellite)
    {
        bCharacterCrashedInSatellite = true;
    }

    if (CharacterRef && CurrentCharacterLocation.Z < LowerBound || CurrentCharacterLocation.Z > UpperBound)
    {
        CharacterRef->Fall();
        bIsCharacterFall = true;
    }

    bool bOutOfBounds = CheckForMeteorsOutOfBounds();

    if (bOutOfBounds) {
        CharacterRef->EarthDestroyed();
    }

    if (bIsCharacterFall || bOutOfBounds || bCharacterCrashedInSatellite)
    {
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
    ResetBackground();
    nextIncreaseScores = difficultyStep;
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
                GetWorld()->GetTimerManager().SetTimer(EnableContinueTimerHandle, [this]()
                {
                    GameOverWidget->EnableContinue();
                }, 2.0f, false);
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

void AMightyHeroGameModeBase::ResetBackground()
{
    if (BackgroundController)
    {
        BackgroundController->ResetBackground();
    }
}

void AMightyHeroGameModeBase::IncreaseDifficulty()
{
    if (DataController)
    {
        if (CharacterRef->GetPlayerState<AMightyHeroPlayerState>()->GetScores() > nextIncreaseScores)
        {
            MeteorController->IncreaseDifficulty();
            CharacterRef->IncreaseDifficulty();

            nextIncreaseScores += difficultyStep;
        }
    }
}

ASoundController* AMightyHeroGameModeBase::GetSoundController()
{
    if (SoundController)
    {
        return SoundController;
    }
    
    return nullptr;
}
