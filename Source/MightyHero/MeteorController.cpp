


#include "MeteorController.h"
#include "Kismet/GameplayStatics.h"
#include "MeteorActor.h"
#include "CharacterBase.h"
#include "EngineUtils.h"

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
			SpawnMeteor();
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

		AMeteorActor* Meteor = World->SpawnActor<AMeteorActor>(MeteorActorClass, SpawnLocation, Rotation, SpawnParams);
		Meteor->SetFallVelocity(100);

		NextSpawnDistance += SpawnStepDistance;
	}
}

void AMeteorController::DestroyAllMeteors()
{
	for (TActorIterator<AMeteorActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		AMeteorActor* Meteor = *ActorItr;
		Meteor->Destruction();
	}
}

void AMeteorController::ResetController()
{
	FirstSpawnDistance = 1000.f;
	SpawnStepDistance = 1600.f;
	NextSpawnDistance = FirstSpawnDistance;
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



