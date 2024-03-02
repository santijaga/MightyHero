// Fill out your copyright notice in the Description page of Project Settings.


#include "MightyHeroGameModeBase.h"

#include "BackgroundController.h"
#include "CharacterBase.h"
#include "CoinsController.h"
#include "CollectablesController.h"
#include "CollectionWidget.h"
#include "EngineUtils.h"
#include "GameDataController.h"
#include "GameOverWidgetBase.h"
#include "GameplayWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "MainWidgetBase.h"
#include "MeteorController.h"
#include "MightyHeroPlayerController.h"
#include "MightyHeroPlayerState.h"
#include "SoundController.h"
#include "TrackCameraActorBase.h"

#include "UI/UIController.h"

void AMightyHeroGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    SetupControllers();

    InitUI();

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        FString PlayerStartTag = "MainStart";
        MainPlayerStart = FindPlayerStart(PC, PlayerStartTag);
    }

    if (SoundController)
    {
        SoundController->SetSound(DataController->LoadSoundSetting());
    }

    if (DataController)
    {
        if (UClass* ActivePawnClass = DataController->LoadActivePawnClass())
        {
            if (UGameplayStatics::GetPlayerPawn(this, 0)->GetClass() != ActivePawnClass)
            {
                RefreshPawn();
            }
        }
    }
}

void AMightyHeroGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsGameOver)
    {
        CheckForCollectablesOutOfBounds();
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

    if (UGameplayStatics::GetPlayerPawn(this, 0))
    {
        Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(this, 0))->StartGameplay();
    }

    if (MeteorController)
    {
        MeteorController->ResetController();
    }

    if (CollectablesController)
    {
        CollectablesController->ResetController();
        CollectablesController->StartGameplay();
    }

    DifficultyLevel = 0;
    nextIncreaseScores = difficultyStep;
    bIsGameOver = false;
    bIsCharacterFall = false;
}

void AMightyHeroGameModeBase::GameOver()
{
    UE_LOG(LogTemp, Warning, TEXT("Game is over"));

    bIsGameOver = true;
    MeteorController->DestroyAllMeteors(false);
    CollectablesController->StopGameplay();
    HideGameplayWidget();
    ShowGameOverWidget();
    SaveHighScore();
    SaveCoins();
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
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (MainWidgetClass)
        {
            MainWidget = Cast<UMainWidgetBase>(CreateWidget<UUserWidget>(PC, MainWidgetClass));
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
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (GameplayWidgetClass)
        {
            GameplayWidget = Cast<UGameplayWidgetBase>(CreateWidget<UUserWidget>(PC, GameplayWidgetClass));
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
    if (UGameplayStatics::GetPlayerPawn(this, 0))
    {
        FVector CurrentCharacterLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();

        return CurrentCharacterLocation;
    }

    return FVector(0, 0, 0);
}

void AMightyHeroGameModeBase::CheckForGameOver()
{
    FVector CurrentCharacterLocation = TrackCharacterLocation();
    ACharacterBase* CharacterRef = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(this, 0));

    bool bCharacterCrashedInSatellite = false;
    if (CharacterRef)
    {
        if (CharacterRef->bCrashedInSatellite)
        {
            bCharacterCrashedInSatellite = true;
        }
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

void AMightyHeroGameModeBase::SetupControllers()
{
    if (GetWorld())
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = this;
        FTransform SpawnTransform = FTransform(FRotator(0), FVector(0), FVector(1));

        if (MeteorControllerClass)
        {
            MeteorController = GetWorld()->SpawnActor<AMeteorController>(MeteorControllerClass, SpawnTransform, SpawnParameters);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[Mighty Hero Game Mode Base] Setup Controllers: MeteorControllerClass not set!"));
        }

        if (BackgroundControllerClass)
        {
            BackgroundController = GetWorld()->SpawnActor<ABackgroundController>(BackgroundControllerClass, SpawnTransform, SpawnParameters);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[Mighty Hero Game Mode Base] Setup Controllers: BackgroundControllerClass not set!"));
        }
        
        if (SoundControllerClass)
        {
            SoundController = GetWorld()->SpawnActor<ASoundController>(SoundControllerClass, SpawnTransform, SpawnParameters);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[Mighty Hero Game Mode Base] Setup Controllers: SoundControllerClass not set!"));
        }
        
        if (CollectablesControllerClass)
        {
            CollectablesController = GetWorld()->SpawnActor<ACollectablesController>(CollectablesControllerClass, SpawnTransform, SpawnParameters);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[Mighty Hero Game Mode Base] Setup Controllers: CollectablesControllerClass not set!"));
        }
        
        if (CoinsControllerClass)
        {
            CoinsController = GetWorld()->SpawnActor<ACoinsController>(CoinsControllerClass, SpawnTransform, SpawnParameters);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[Mighty Hero Game Mode Base] Setup Controllers: CoinsControllerClass not set!"));
        }

        DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass());
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

void AMightyHeroGameModeBase::CheckForCollectablesOutOfBounds()
{
    if (CollectablesController)
    {
        double XBound = MainCamera->GetActorLocation().X + BackBound;
        CollectablesController->DestroyAnyCollectablesOutOfBounds(XBound, LowerBound);
    }
}

void AMightyHeroGameModeBase::ResetGame()
{
    HideGameOverWidget();
    ShowMainWidget();
    ResetCharacter();
    ResetScores();
    ResetBackground();
    ResetCoins();
    nextIncreaseScores = difficultyStep;
}

void AMightyHeroGameModeBase::ShowGameOverWidget()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (GameOverWidgetClass)
        {
            GameOverWidget = Cast<UGameOverWidgetBase>(CreateWidget<UUserWidget>(PC, GameOverWidgetClass));
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
    if (ACharacterBase* CharacterRef = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(this, 0)))
    {
        CharacterRef->SetActorLocation(MainPlayerStart->GetActorLocation());
        CharacterRef->ResetCharacter();
    }
}

void AMightyHeroGameModeBase::SaveHighScore()
{
    if (UGameplayStatics::GetPlayerPawn(this, 0))
    {
        int32 Scores = UGameplayStatics::GetPlayerPawn(this, 0)->GetPlayerState<AMightyHeroPlayerState>()->GetScores();
        int32 HighScores = DataController->LoadHighScore();

        if (Scores > HighScores)
        {
            DataController->SaveHighScore(Scores);
        }
    }
}

void AMightyHeroGameModeBase::SaveCoins()
{
    if (UGameplayStatics::GetPlayerPawn(this, 0))
    {
        int32 CollectedCoins = UGameplayStatics::GetPlayerPawn(this, 0)->GetPlayerState<AMightyHeroPlayerState>()->GetCoins();
        int32 OwnedCoins = DataController->LoadCoins();

        DataController->SaveCoins(CollectedCoins + OwnedCoins);
    }
}

void AMightyHeroGameModeBase::ResetScores()
{
    if (UGameplayStatics::GetPlayerPawn(this, 0))
    {
        UGameplayStatics::GetPlayerPawn(this, 0)->GetPlayerState<AMightyHeroPlayerState>()->ResetScores();
    }
}

void AMightyHeroGameModeBase::ResetCoins()
{
    if (UGameplayStatics::GetPlayerPawn(this, 0))
    {
        UGameplayStatics::GetPlayerPawn(this, 0)->GetPlayerState<AMightyHeroPlayerState>()->ResetCoins();
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
        if (UGameplayStatics::GetPlayerPawn(this, 0)->GetPlayerState<AMightyHeroPlayerState>()->GetScores() > nextIncreaseScores)
        {
            DifficultyLevel++;
            MeteorController->IncreaseDifficulty(DifficultyLevel);
            if (CollectablesController)
            {
                CollectablesController->IncreaseDifficulty(DifficultyLevel);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Collectables Controller not connected!"));
            }
            Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(this, 0))->IncreaseDifficulty(DifficultyLevel);

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

ACollectablesController* AMightyHeroGameModeBase::GetCollectablesController()
{
    if (CollectablesController)
    {
        return CollectablesController;
    }

    return nullptr;
}

void AMightyHeroGameModeBase::OpenCollection()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (CollectionWidgetClass)
        {
            CollectionWidget = Cast<UCollectionWidget>(CreateWidget<UUserWidget>(PC, CollectionWidgetClass));
            if (CollectionWidget)
            {
                CollectionWidget->ShowWidget();
                RefreshCollection();
            }
        }
    }
}

void AMightyHeroGameModeBase::RefreshPawn()
{
    if (GetWorld())
    {
        if (DataController)
        {
            if (UClass* ClassToSpawn = DataController->LoadActivePawnClass())
            {
                if (UGameplayStatics::GetPlayerPawn(this, 0))
                {
                    FTransform SpawnTransform = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorTransform();
                    
                    FActorSpawnParameters SpawnParams;
                    SpawnParams.Owner = this;

                    if (ACharacterBase* SpawnedActor = GetWorld()->SpawnActor<ACharacterBase>(ClassToSpawn, SpawnTransform, SpawnParams))
                    {
                        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
                        {
                            AActor* PreviousPawn = UGameplayStatics::GetPlayerPawn(this, 0);

                            PC->UnPossess();

                            PreviousPawn->Destroy();

                            PC->Possess(SpawnedActor);

                            ResetCharacter();

                            MainCamera->ResetCamera();

                            PC->SetViewTargetWithBlend(MainCamera, 0.0f);
                        }
                    }
                }
            }
        }
    }
}

void AMightyHeroGameModeBase::RefreshCollection()
{
    if (CollectionWidget)
    {
        CollectionWidget->RefreshAllCards();
    }
}

ACoinsController* AMightyHeroGameModeBase::GetCoinsController()
{
    if (CoinsController)
    {
        return CoinsController;
    }

    return nullptr;
}

int32 AMightyHeroGameModeBase::GetCurrentDifficulty()
{
    return DifficultyLevel;
}

AMeteorController* AMightyHeroGameModeBase::GetMeteorController()
{
    if (MeteorController)
    {
        return MeteorController;
    }

    return nullptr;
}
