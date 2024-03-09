// Fill out your copyright notice in the Description page of Project Settings.


#include "MightyHeroGameModeBase.h"

#include "BackgroundController.h"
#include "CharacterBase.h"
#include "CoinsController.h"
#include "CollectablesController.h"
#include "EngineUtils.h"
#include "GameDataController.h"
#include "Kismet/GameplayStatics.h"
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
    
    if (UIController)
    {
        UIController->ShowMainMenuUI();
    }

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

/*
* Controllers
*/

void AMightyHeroGameModeBase::SetupControllers()
{
    if (GetWorld())
    {
        SetupController(MeteorControllerClass, MeteorController);
        SetupController(BackgroundControllerClass, BackgroundController);
        SetupController(SoundControllerClass, SoundController);
        SetupController(CollectablesControllerClass, CollectablesController);
        SetupController(CoinsControllerClass, CoinsController);
        SetupController(UIControllerClass, UIController);

        DataController = NewObject<UGameDataController>(this, UGameDataController::StaticClass());
    }
}

template<typename ControllerClass>
void AMightyHeroGameModeBase::SetupController(TSubclassOf<ControllerClass>& ControllerClassRef, ControllerClass*& ControllerInstance)
{
    if (ControllerClassRef)
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = this;
        FTransform SpawnTransform = FTransform(FRotator(0), FVector(0), FVector(1));
        ControllerInstance = GetWorld()->SpawnActor<ControllerClass>(ControllerClassRef, SpawnTransform, SpawnParameters);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[Mighty Hero Game Mode Base] Setup Controllers: %s not set!"), *ControllerClassRef->GetName());
    }
}

// UIController

AUIController* AMightyHeroGameModeBase::GetUIController()
{
    if (UIController)
    {
        return UIController;
    }

    return nullptr;
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

void AMightyHeroGameModeBase::StartGameplay()
{
    if (UIController)
    {
        UIController->ShowGameplayUI();
    }

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
    if (MeteorController)
    {
        MeteorController->DestroyAllMeteors(false);
    }

    if (CollectablesController)
    {
        CollectablesController->StopGameplay();
    }

    if (UIController)
    {
        UIController->ShowGameOverUI(true);
    }

    SaveHighScore();
    SaveCoins();
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

bool AMightyHeroGameModeBase::IsGameOver()
{
    return bIsGameOver;
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

    bool bMeteorIsOutOfBounds = CheckForMeteorsOutOfBounds();

    if (bMeteorIsOutOfBounds) {
        CharacterRef->EarthDestroyed();
    }

    if (bIsCharacterFall || bMeteorIsOutOfBounds || bCharacterCrashedInSatellite)
    {
        if (bIsCharacterFall)
        {
            UE_LOG(LogTemp, Warning, TEXT("Game Over due character fall"));
        }

        if (bMeteorIsOutOfBounds)
        {
            UE_LOG(LogTemp, Warning, TEXT("Game Over due meteor was out of bounds"));
        }

        if (bCharacterCrashedInSatellite)
        {
            UE_LOG(LogTemp, Warning, TEXT("Game Over due character destroyed satellite"));
        }

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
    if (UIController)
    {
        UIController->ShowMainMenuUI();
    }

    ResetCharacter();
    ResetScores();
    ResetBackground();
    ResetCoins();
    nextIncreaseScores = difficultyStep;
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
