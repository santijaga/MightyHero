


#include "UI/Widgets/RPGGameplayUserWidget.h"

#include "Core/MightyHeroPlayerState.h"
#include "Core/MightyHeroRPGGameModeBase.h"
#include "Kismet/GameplayStatics.h"

/*
* Interface
*/
float URPGGameplayUserWidget::GetHealthPercentage()
{
	if (AMightyHeroPlayerState* PlayerState = Cast<AMightyHeroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
	{
		int32 MaxHP = PlayerState->GetMaxHitPoints();
		if (MaxHP != 0)
		{
			int32 CurrentHP = PlayerState->GetCurrentHitPoints();
			float Percentage = static_cast<float>(CurrentHP) / MaxHP;
			return Percentage;
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
		int32 MaxShieldPoints = PlayerState->GetMaxShieldPoints();
		if (MaxShieldPoints != 0)
		{
			int32 CurrentShieldPoints = PlayerState->GetCurrentShieldPoints();
			float Percentage = static_cast<float>(CurrentShieldPoints) / MaxShieldPoints;
			return Percentage;
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
