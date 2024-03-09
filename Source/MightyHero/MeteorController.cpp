


#include "MeteorController.h"
#include "Kismet/GameplayStatics.h"
#include "MeteorActor.h"
#include "SatelliteActor.h"
#include "CharacterBase.h"
#include "EngineUtils.h"
#include "Math/UnrealMathUtility.h"
#include "MightyHeroPlayerState.h"

// Sets default values
AMeteorController::AMeteorController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMeteorController::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	currentSatelliteSpawnChance = defaultSatelliteSpawnChance;
	DefaultMinFallVelocity = minFallVelocity;
	DefaultMaxFallVelocity = maxFallVelocity;
}

// Called every frame
void AMeteorController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation().X > NextSpawnDistance)
	{
		if (ShouldSpawnSatellite())
		{
			SpawnSatellite();
		}
		else
		{
			SpawnMeteor();
		}
	}
}

float AMeteorController::GetDeltaSpawnLocation()
{
	return (PenultimateSpawnLocation + LastSpawnLocation) * 0.5f + CurrentSpawnStepDistance * 2;
}

void AMeteorController::SpawnMeteor()
{
	if (World && MeteorActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = GetActorLocation();

		SpawnLocation.X = NextSpawnDistance + SpawnThreshold;
		SpawnLocation.Z = ZThreshold;

		PenultimateSpawnLocation = LastSpawnLocation;
		LastSpawnLocation = SpawnLocation.X;

		FRotator Rotation = GetActorRotation();
		FVector Scale = FVector(CurrentScale);
		FTransform SpawnTransform = FTransform(Rotation, SpawnLocation, Scale);
		AMeteorActor* Meteor = World->SpawnActor<AMeteorActor>(MeteorActorClass, SpawnTransform, SpawnParams);

		Meteor->SetFallVelocity(FMath::RandRange(minFallVelocity, maxFallVelocity));

		NextSpawnDistance += CurrentSpawnStepDistance;

		bJustSpawnedSattelite = false;
	}
}

void AMeteorController::DestroyAllMeteors(bool BlowerImpact)
{
	for (TActorIterator<AMeteorActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		AMeteorActor* Meteor = *ActorItr;

		if (BlowerImpact)
		{
			UGameplayStatics::GetPlayerPawn(this, 0)->GetPlayerState<AMightyHeroPlayerState>()->AddScore();
			Meteor->Destruction(true);
		}
		else
		{
			Meteor->Destruction();
		}

	}

	if (!BlowerImpact)
	{
		for (TActorIterator<ASatelliteActor> ActorItr(World); ActorItr; ++ActorItr)
		{
			ASatelliteActor* Satellite = *ActorItr;
			Satellite->Destruction();
		}
	}
}

void AMeteorController::ResetController()
{
	difficultyLevel = 0;
	CurrentSpawnStepDistance = SpawnStepDistance;
	CurrentScale = DefaultScale;
	NextSpawnDistance = FirstSpawnDistance;
	currentSatelliteSpawnChance = defaultSatelliteSpawnChance;
	minFallVelocity = DefaultMinFallVelocity;
	maxFallVelocity = DefaultMaxFallVelocity;

	DestroyAllMeteors(false);
}

bool AMeteorController::ShouldSpawnSatellite()
{
	if (bJustSpawnedSattelite)
	{
		return false;
	}
	
	if (difficultyLevel >= satelliteSpawnDifficultyLevel)
	{
		int32 rollResult = FMath::RandRange(1, 100);
		if (rollResult < currentSatelliteSpawnChance)
		{
			currentSatelliteSpawnChance = defaultSatelliteSpawnChance;
			return true;
		}
		else
		{
			currentSatelliteSpawnChance += satelliteSpawnChanceIncreaceStep;
			return false;
		}
	}

	return false;
}

void AMeteorController::SpawnSatellite()
{
	if (World && SatelliteActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = GetActorLocation();

		SpawnLocation.X = NextSpawnDistance + SpawnThreshold;

		PenultimateSpawnLocation = LastSpawnLocation;
		LastSpawnLocation = SpawnLocation.X;

		SpawnLocation.Z = FMath::RandRange(satelliteSpawnMinHeight, satelliteSpawnMaxHeight);

		FRotator Rotation = GetActorRotation();
		ASatelliteActor* satelliteActor = World->SpawnActor<ASatelliteActor>(SatelliteActorClass, SpawnLocation, Rotation, SpawnParams);

		NextSpawnDistance += CurrentSpawnStepDistance;

		bJustSpawnedSattelite = true;
	}
}

void AMeteorController::IncreaseDifficulty(int32 NewDifficulty)
{
	difficultyLevel = NewDifficulty;;

	if (minFallVelocityPerLevel.IsValidIndex(difficultyLevel))
	{
		minFallVelocity = minFallVelocityPerLevel[difficultyLevel];
	}

	if (maxFallVelocityPerLevel.IsValidIndex(difficultyLevel))
	{
		maxFallVelocity = maxFallVelocityPerLevel[difficultyLevel];
	}

	if (scalePerLevel.IsValidIndex(difficultyLevel))
	{
		CurrentScale = scalePerLevel[difficultyLevel];
	}

	if (spawnStepDistancePerLevel.IsValidIndex(difficultyLevel))
	{
		CurrentSpawnStepDistance = spawnStepDistancePerLevel[difficultyLevel];
	}
}

bool AMeteorController::HasAnyMeteorsOutOfBounds(double XBound, double ZBound)
{
	bool isAnyMeteorTooFar = false;
	bool isAnyMeteorTooLow = false;

	for (TActorIterator<AMeteorActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		AMeteorActor* Meteor = *ActorItr;
		FVector MeteorLocation = Meteor->GetActorLocation();

		if (MeteorLocation.X < XBound)
		{
			isAnyMeteorTooFar = true;
			UE_LOG(LogTemp, Warning, TEXT("Meteor is too far"));
		}

		if (MeteorLocation.Z < ZBound)
		{
			isAnyMeteorTooLow = true;
			UE_LOG(LogTemp, Warning, TEXT("Meteor is too low"));
		}

		if (isAnyMeteorTooFar || isAnyMeteorTooLow)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s - failed meteor location"), *MeteorLocation.ToString());
		}
	}

	return isAnyMeteorTooFar || isAnyMeteorTooLow;
}



