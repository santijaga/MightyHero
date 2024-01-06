#include "MightyHeroPlayerState.h"

AMightyHeroPlayerState::AMightyHeroPlayerState()
{
	UE_LOG(LogTemp, Warning, TEXT("Player State Constructed"));
}

void AMightyHeroPlayerState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[Player State] Begin Play runs."));
}

void AMightyHeroPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMightyHeroPlayerState::AddScore()
{
	Scores++;
}

int32 AMightyHeroPlayerState::GetScores()
{
	return Scores;
}