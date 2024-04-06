


#include "UI/Widgets/RPGGameplayUserWidget.h"

#include "Core/MightyHeroPlayerState.h"
#include "Core/MightyHeroRPGGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Missions/MissionsController.h"

/*
* Interface
*/
float URPGGameplayUserWidget::GetHealthPercentage()
{
	if (AMightyHeroPlayerState* PlayerState = Cast<AMightyHeroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
	{
		if (int32 MaxHP = PlayerState->GetMaxHitPoints() != 0)
		{
			int32 CurrentHP = PlayerState->GetCurrentHitPoints();
			return (CurrentHP / MaxHP) * 100;
		}
	}

	return 0.0f;
}

int32 URPGGameplayUserWidget::GetHitPoints()
{
	if (AMightyHeroPlayerState* PS = Cast<AMightyHeroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
	{
		return PS->GetCurrentHitPoints();
	}

	return 0;
}

FString URPGGameplayUserWidget::GetMissionDescription()
{
	if (AMightyHeroRPGGameModeBase* GM = Cast<AMightyHeroRPGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		return Cast<AMissionsController>(GM->GetMissionsController())->GetMissionDescription();
	}

	return "";
}

FString URPGGameplayUserWidget::GetMissionName()
{
	if (AMightyHeroRPGGameModeBase* GM = Cast<AMightyHeroRPGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		return Cast<AMissionsController>(GM->GetMissionsController())->GetMissionName();
	}
	
	return "";
}

int32 URPGGameplayUserWidget::GetScores()
{
	if (AMightyHeroPlayerState* PlayerState = Cast<AMightyHeroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
	{
		return PlayerState->GetScores();
	}

	return 0;
}

float URPGGameplayUserWidget::GetShieldPercentage()
{
	if (AMightyHeroPlayerState* PlayerState = Cast<AMightyHeroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
	{
		if (int32 MaxShieldPoints = PlayerState->GetMaxShieldPoints() != 0)
		{
			int32 CurrentShieldPoints = PlayerState->GetCurrentShieldPoints();
			return (CurrentShieldPoints / MaxShieldPoints) * 100;
		}
	}

	return 0.0f;
}

int32 URPGGameplayUserWidget::GetShieldPoints()
{
	if (AMightyHeroPlayerState* PS = Cast<AMightyHeroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
	{
		return PS->GetCurrentShieldPoints();
	}

	return 0;
}
