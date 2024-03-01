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

void AMightyHeroGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = Cast<AMightyHeroPlayerController>(GetWorld()->GetFirstPlayerController());

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

    if (World)
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = this;
        CollectablesController = World->SpawnActor<ACollectablesController>(CollectablesControllerClass, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParameters);
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

    if (World)
    {
        if (CoinsControllerClass)
        {
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.Owner = this;
            CoinsController = World->SpawnActor<ACoinsController>(CoinsControllerClass, FVector(0), FRotator(0), SpawnParameters);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[AMightyHeroGameModeBase] Coins Controller not setup!"));
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
    if (PlayerController)
    {
        if (CollectionWidgetClass)
        {
            CollectionWidget = Cast<UCollectionWidget>(CreateWidget<UUserWidget>(PlayerController, CollectionWidgetClass));
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
                        if (PlayerController)
                        {
                            AActor* PreviousPawn = UGameplayStatics::GetPlayerPawn(this, 0);

                            PlayerController->UnPossess();

                            PreviousPawn->Destroy();

                            PlayerController->Possess(SpawnedActor);

                            ResetCharacter();

                            MainCamera->ResetCamera();

                            PlayerController->SetViewTargetWithBlend(MainCamera, 0.0f);
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
