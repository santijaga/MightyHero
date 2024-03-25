


#include "CollectablesController.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterBase.h"
#include "Blower.h"
#include "EngineUtils.h"
#include "MightyHeroGameModeBase.h"
#include "MeteorController.h"
#include "AuraCollectable.h"
#include "AuraActor.h"
#include "Sound/SoundController.h"
#include "LaserActor.h"

// Sets default values
ACollectablesController::ACollectablesController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACollectablesController::BeginPlay()
{
	Super::BeginPlay();
	
	World = GetWorld();

	CurrentBlowerSpawnChance = DefaultBlowerSpawnChance;
}

// Called every frame
void ACollectablesController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGameplayStatics::GetPlayerPawn(this, 0))
	{
		if (UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation().X > NextSpawnDistance)
		{
			if (ShouldSpawnBlower())
			{
				SpawnBlower();
			}
			else
			{
				NextSpawnDistance += CurrentSpawnStepDistance;
			}
		}
	}
}

void ACollectablesController::SpawnBlower()
{
	if (World && BlowerActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = GetActorLocation();

		SpawnLocation.X = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetMeteorController()->GetDeltaSpawnLocation();;
		SpawnLocation.Z = FMath::RandRange(BlowerSpawnMinHeight, BlowerSpawnMaxHeight);

		FRotator Rotation = GetActorRotation();
		ABlower* Blower = World->SpawnActor<ABlower>(BlowerActorClass, SpawnLocation, Rotation, SpawnParams);

		NextSpawnDistance += CurrentSpawnStepDistance;
	}
}

void ACollectablesController::RunAllTimers()
{
	if (World && IsAbilityCollectablesActive)
	{
		World->GetTimerManager().SetTimer(FirstAbilitiesCollectablesTimerHandle, this, &ACollectablesController::SpawnFirstAbilitiesCollectable, FirstAbilitiesCollectableSpawnTimer, false);
	}
}

void ACollectablesController::ClearAllTimers()
{
	if (World)
	{
		World->GetTimerManager().ClearTimer(FirstAbilitiesCollectablesTimerHandle);
		World->GetTimerManager().ClearTimer(AbilitiesCollectablesTimerHandle);
	}
}

void ACollectablesController::StartGameplay()
{
	RunAllTimers();
}

void ACollectablesController::StopGameplay()
{
	DestroyAllCollectables();
	ClearAllTimers();
}

void ACollectablesController::DestroyAllCollectables()
{
	for (TActorIterator<ACollectableBase> ActorItr(World); ActorItr; ++ActorItr)
	{
		if (ACollectableBase* Collectable = *ActorItr)
		{
			Collectable->Collect();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Collectable cannot be accessed!"));
		}
	}

	for (TActorIterator<AAuraActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		if (AAuraActor* Aura = *ActorItr)
		{
			Aura->Deactivate(false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Aura cannot be accessed!"));
		}
	}

	for (TActorIterator<ALaserActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		if (ALaserActor* Laser = *ActorItr)
		{
			Laser->Deactivate(false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Laser cannot be accessed!"));
		}
	}
}

void ACollectablesController::ResetController()
{
	difficultyLevel = 0;
	CurrentSpawnStepDistance = SpawnStepDistance;
	NextSpawnDistance = FirstSpawnDistance;
	CurrentBlowerSpawnChance = DefaultBlowerSpawnChance;
}

bool ACollectablesController::ShouldSpawnBlower()
{
	if (difficultyLevel >= BlowerSpawnDifficultyLevel)
	{
		int32 rollResult = FMath::RandRange(1, 100);
		if (rollResult < CurrentBlowerSpawnChance)
		{
			CurrentBlowerSpawnChance = DefaultBlowerSpawnChance;
			return true;
		}
		else
		{
			CurrentBlowerSpawnChance += BlowerSpawnChanceIncreaceStep;
			return false;
		}
	}

	return false;
}

void ACollectablesController::IncreaseDifficulty(int32 NewDifficulty)
{
	difficultyLevel = NewDifficulty;

	if (SpawnStepDistancePerLevel.IsValidIndex(difficultyLevel))
	{
		CurrentSpawnStepDistance = SpawnStepDistancePerLevel[difficultyLevel];
	}
}

void ACollectablesController::ScheduleNextCollectable()
{
	SetupNextAbilitiesCollectableSpawn();
}

void ACollectablesController::SpawnFirstAbilitiesCollectable()
{
	SpawnRandomAbilitiesCollectable();
}

void ACollectablesController::SpawnRandomAbilitiesCollectable()
{
	int32 RandomCollectableIndex = FMath::RandRange(0, AbilitiesCollectables.Num() - 1);

	if (AbilitiesCollectables.IsValidIndex(RandomCollectableIndex))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector CharacterLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();

		
		FVector SpawnLocation = FVector(0);
		SpawnLocation.X = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetMeteorController()->GetDeltaSpawnLocation();
		SpawnLocation.Z = FMath::RandRange(AbilitiesCollectablesSpawnMinHeight, AbilitiesCollectablesMaxHeight);

		FRotator Rotation = GetActorRotation();
		ACollectableBase* AbilitiesCollectable = World->SpawnActor<ACollectableBase>(AbilitiesCollectables[RandomCollectableIndex], SpawnLocation, Rotation, SpawnParams);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not valid abilities index"));
	}
}

void ACollectablesController::SetupNextAbilitiesCollectableSpawn()
{
	if (World)
	{
		World->GetTimerManager().SetTimer(AbilitiesCollectablesTimerHandle, this, &ACollectablesController::SpawnRandomAbilitiesCollectable, AbilitiesCollectableSpawnTimer, false);
	}
}

void ACollectablesController::DestroyAnyCollectablesOutOfBounds(double XBound, double ZBound)
{
	for (TActorIterator<ACollectableBase> ActorItr(World); ActorItr; ++ActorItr)
	{
		ACollectableBase* Collectable = *ActorItr;

		if (AbilitiesCollectables.Contains(Collectable->GetClass()))
		{
			ScheduleNextCollectable();
		}

		FVector CollectableLocation = Collectable->GetActorLocation();

		if (CollectableLocation.X < XBound || CollectableLocation.Z < ZBound)
		{
			Collectable->DestroyCollectable();
		}
	}
}

void ACollectablesController::CollectableBehaviour(ACollectableBase* ActiveCollectable)
{
	if (ABlower* BlowerActor = Cast<ABlower>(ActiveCollectable))
	{
		BlowerBehaviour(BlowerActor);
	}

	if (AAuraCollectable* AuraCollectable = Cast<AAuraCollectable>(ActiveCollectable))
	{
		AuraBehaviour(AuraCollectable);
	}

	ActiveCollectable->Collect();
}

void ACollectablesController::BlowerBehaviour(ABlower* ActiveBlower)
{
	ActiveBlower->Collect();

	if (AMightyHeroGameModeBase* GameMode = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		if (AMeteorController* MeteorController = GameMode->GetMeteorController())
		{
			MeteorController->DestroyAllMeteors(true);
		}
	}
}

void ACollectablesController::AuraBehaviour(AAuraCollectable* ActiveAura)
{
	ActiveAura->Collect();
	if (AuraActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation, AuraActorScale);

		AAuraActor* SpawnedAuraActor = GetWorld()->SpawnActor<AAuraActor>(AuraActorClass, SpawnTransform, SpawnParams);
		SpawnedAuraActor->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Aura Actor Class not set."));
	}
}

void ACollectablesController::PlayLongCollectSound()
{
	if (UWorld* GameWorld = GetWorld())
	{
		if (AMightyHeroGameModeBase* GameMode = Cast<AMightyHeroGameModeBase>(GameWorld->GetAuthGameMode()))
		{
			if (ASoundController* SoundController = GameMode->GetSoundController())
			{
				SoundController->PlayLongCollectCue();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Sound Component Reference missing!"));
			}
		}
	}
}