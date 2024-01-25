


#include "MeteorController.h"
#include "Kismet/GameplayStatics.h"
#include "MeteorActor.h"
#include "SatelliteActor.h"
#include "CharacterBase.h"
#include "EngineUtils.h"
#include "Math/UnrealMathUtility.h"

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

	CharacterRef = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	World = GetWorld();
}

// Called every frame
void AMeteorController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CharacterRef)
	{
		if (CharacterRef->GetActorLocation().X > NextSpawnDistance)
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

		FRotator Rotation = GetActorRotation();
		FVector Scale = FVector(CurrentScale);
		FTransform SpawnTransform = FTransform(Rotation, SpawnLocation, Scale);
		AMeteorActor* Meteor = World->SpawnActor<AMeteorActor>(MeteorActorClass, SpawnTransform, SpawnParams);

		Meteor->SetFallVelocity(FMath::RandRange(minFallVelocity, maxFallVelocity));

		NextSpawnDistance += CurrentSpawnStepDistance;
	}
}

void AMeteorController::DestroyAllMeteors()
{
	for (TActorIterator<AMeteorActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		AMeteorActor* Meteor = *ActorItr;
		Meteor->Destruction();
	}

	for (TActorIterator<ASatelliteActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		ASatelliteActor* Satellite = *ActorItr;
		Satellite->Destruction();
	}
}

void AMeteorController::ResetController()
{
	difficultyLevel = 0;
	CurrentSpawnStepDistance = SpawnStepDistance;
	CurrentScale = DefaultScale;
	NextSpawnDistance = FirstSpawnDistance;
	currentSatelliteSpawnChance = defaultSatelliteSpawnChance;
}

bool AMeteorController::ShouldSpawnSatellite()
{
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
		SpawnLocation.Z = FMath::RandRange(satelliteSpawnMinHeight, satelliteSpawnMaxHeight);

		FRotator Rotation = GetActorRotation();
		ASatelliteActor* satelliteActor = World->SpawnActor<ASatelliteActor>(SatelliteActorClass, SpawnLocation, Rotation, SpawnParams);

		NextSpawnDistance += CurrentSpawnStepDistance;
	}
}

void AMeteorController::IncreaseDifficulty()
{
	difficultyLevel++;

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
	bool isAnyMeteorsOutOfBounds = false;

	for (TActorIterator<AMeteorActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		AMeteorActor* Meteor = *ActorItr;
		FVector MeteorLocation = Meteor->GetActorLocation();

		if (MeteorLocation.X < XBound || MeteorLocation.Z < ZBound)
		{
			isAnyMeteorsOutOfBounds = true;
			break;
		}
	}

	return isAnyMeteorsOutOfBounds;
}



