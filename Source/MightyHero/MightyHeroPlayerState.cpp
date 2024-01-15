#include "MightyHeroPlayerState.h"

AMightyHeroPlayerState::AMightyHeroPlayerState()
{
}

void AMightyHeroPlayerState::BeginPlay()
{
	Super::BeginPlay();
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

void AMightyHeroPlayerState::ResetScores()
{
	Scores = 0;
}
