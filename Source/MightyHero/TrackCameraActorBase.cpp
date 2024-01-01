#include "TrackCameraActorBase.h"
#include "CharacterBase.h"

ATrackCameraActorBase::ATrackCameraActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATrackCameraActorBase::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = Cast<ACharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CharacterCurrentXPosition = GetCharacterCurrentXLocation();
}

void ATrackCameraActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TrackCharacter();
}

void ATrackCameraActorBase::TrackCharacter()
{
	if (CharacterRef)
	{
		double NewCharacterXPosition = GetCharacterCurrentXLocation();
		double DeltaX = NewCharacterXPosition - CharacterCurrentXPosition;
		
		FVector CurrentLocation = GetActorLocation();

		FVector NewLocation = FVector(CurrentLocation.X + DeltaX, CurrentLocation.Y, CurrentLocation.Z);
		SetActorLocation(NewLocation);

		CharacterCurrentXPosition = GetCharacterCurrentXLocation();
	}
}

double ATrackCameraActorBase::GetCharacterCurrentXLocation()
{
	if (CharacterRef)
	{
		FVector CharacterLocation = CharacterRef->GetActorLocation();
		return CharacterLocation.X;
	}

	return 0;
}