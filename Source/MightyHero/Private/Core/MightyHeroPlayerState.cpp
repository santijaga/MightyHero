#include "Core/MightyHeroPlayerState.h"

#include "Core/MightyHeroPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/RPGPawn.h"

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

void AMightyHeroPlayerState::ApplyDamage(float Damage)
{
	float RecievedDamage = Damage;

	if (CurrentShieldPoints > RecievedDamage)
	{
		CurrentShieldPoints -= RecievedDamage;
	}
	else
	{
		RecievedDamage -= CurrentShieldPoints;
		CurrentShieldPoints = 0;
		CurrentHitPoints -= RecievedDamage;
	}
}

int32 AMightyHeroPlayerState::GetScores()
{
	int DistanceScores = CalculateDistanceScores();
	return DistanceScores;
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

int32 AMightyHeroPlayerState::GetCurrentHitPoints()
{
	return CurrentHitPoints;
}

int32 AMightyHeroPlayerState::GetCurrentShieldPoints()
{
	return CurrentShieldPoints;
}

int32 AMightyHeroPlayerState::GetMaxHitPoints()
{
	return MaxHitPoints;
}

int32 AMightyHeroPlayerState::GetMaxShieldPoints()
{
	return MaxShieldPoints;
}

void AMightyHeroPlayerState::ResetCoins()
{
	Coins = 0;
}

void AMightyHeroPlayerState::StartGameplay()
{
	if (AMightyHeroPlayerController* PC = Cast<AMightyHeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		MaxHitPoints = PC->GetVitality() * 10;
		CurrentHitPoints = MaxHitPoints;
		MaxShieldPoints = PC->GetEnergy() * 5;
		CurrentShieldPoints = MaxShieldPoints;
	}
}

/*
* Utils
*/

int AMightyHeroPlayerState::CalculateDistanceScores()
{
	if (ARPGPawn* Pawn = Cast<ARPGPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		float PawnDistance = Pawn->GetDistance();
		return FMath::RoundToInt(PawnDistance / 100);
	}

	return 0;
}