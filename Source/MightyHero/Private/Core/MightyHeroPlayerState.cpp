#include "Core/MightyHeroPlayerState.h"

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

void AMightyHeroPlayerState::AddCoin()
{
	Coins++;
}

int32 AMightyHeroPlayerState::GetCoins()
{
	return Coins;
}

void AMightyHeroPlayerState::ResetCoins()
{
	Coins = 0;
}
