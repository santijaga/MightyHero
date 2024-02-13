


#include "BackgroundController.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "BackgroundActor.h"
#include "GameFramework/Actor.h"

// Sets default values
ABackgroundController::ABackgroundController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABackgroundController::BeginPlay()
{
	Super::BeginPlay();
	
	InitStartSegments();
}

// Called every frame
void ABackgroundController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterHorizontalLocation() > DistanceLimit)
	{
		SpawnNextSegment();
	}
}

double ABackgroundController::GetCharacterHorizontalLocation()
{
	if (UGameplayStatics::GetPlayerPawn(this, 0))
	{
		return UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation().X;
	}

	return 0;
}

void ABackgroundController::SpawnNextSegment()
{
	UWorld* World = GetWorld();
	if (World && BackgroundActorClass != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		FVector Location = GetActorLocation();

		Location.X += Threshold;

		FRotator Rotation = GetActorRotation();

		ABackgroundActor* NewActor = World->SpawnActor<ABackgroundActor>(BackgroundActorClass, Location, Rotation, SpawnParams);

		Segments.Add(NewActor);

		if (Segments.Num() > 5)
		{
			Segments[0]->Destroy();
			Segments[0] = nullptr;
			Segments.Remove(0);
		}

		Threshold += BackgroundSegmentLength;
		DistanceLimit = Threshold - BackgroundSegmentLength - BackgroundSegmentLength / 2;
	}
}

void ABackgroundController::ResetBackground()
{
	DestroyAllSegments();
	InitStartSegments();
}

void ABackgroundController::DestroyAllSegments()
{
	for (ABackgroundActor* Segment : Segments)
	{
		Segment->Destroy();
	}

	Segments.Empty();
}

void ABackgroundController::InitStartSegments()
{
	Threshold = 0;
	DistanceLimit = 0;
	for (int i = 0; i < 3; i++)
	{
		SpawnNextSegment();
	}
}
