#include "Camera/TrackCameraActorBase.h"
#include "../CharacterBase.h"

ATrackCameraActorBase::ATrackCameraActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATrackCameraActorBase::BeginPlay()
{
	Super::BeginPlay();

	DefaultCameraTransform = GetActorTransform();

	CharacterCurrentXPosition = GetCharacterCurrentXLocation();
}

void ATrackCameraActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TrackCharacter();
}

void ATrackCameraActorBase::ResetCamera()
{
	SetActorTransform(DefaultCameraTransform);
}

void ATrackCameraActorBase::TrackCharacter()
{
	if (APawn* ActivePawn = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		FVector CurrentLocation = GetActorLocation();
		SetActorLocation(FVector(CurrentLocation.X + GetCharacterCurrentXLocation() - CharacterCurrentXPosition, CurrentLocation.Y, CurrentLocation.Z));
		CharacterCurrentXPosition = GetCharacterCurrentXLocation();
	}
}

double ATrackCameraActorBase::GetCharacterCurrentXLocation()
{
	if (APawn* ActivePawn = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		return ActivePawn->GetActorLocation().X;
	}

	return 0;
}