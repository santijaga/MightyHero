#include "Camera/TrackCameraActorBase.h"

ATrackCameraActorBase::ATrackCameraActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATrackCameraActorBase::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(InitialLocation);
	SetActorRotation(OffsetRotation);
}

void ATrackCameraActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TrackCharacter();
}

void ATrackCameraActorBase::TrackCharacter()
{
	if (APawn* ActivePawn = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		FVector CurrentLocation = GetActorLocation();
		SetActorLocation(FVector(ActivePawn->GetActorLocation().X + Offset.X, InitialLocation.Y, InitialLocation.Z));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Can't find current pawn"), *GetName());
	}
}