


#include "LaserActor.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "MightyHeroGameModeBase.h"
#include "SoundController.h"
#include "CollectablesController.h"
#include "MeteorActor.h"
#include "EngineUtils.h"
#include "LaserBeamActor.h"

ALaserActor::ALaserActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALaserActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TrackCharacter();
	if (bIsActivated)
	{
		TrackMeteors();
	}
}

void ALaserActor::TrackCharacter()
{
	if (UGameplayStatics::GetPlayerPawn(this, 0))
	{
		SetActorLocation(UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation() + FVector(HorizontalOffset, 0, VerticalOffset));
	}
}

void ALaserActor::OnExpire()
{
	if (UPaperFlipbookComponent* Flipbook = GetRenderComponent())
	{
		Flipbook->SetFlipbook(ExpireFlipbook);
	}

	GetWorld()->GetTimerManager().SetTimer(LaserDurationHandle, this, &ALaserActor::OnDurationEnds, ExpirationDuration, false);
}

void ALaserActor::OnDurationEnds()
{
	if (ASoundController* SoundController = Cast<ASoundController>(Cast<AMightyHeroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSoundController()))
	{
		SoundController->PlayLongCollectCue();
	}

	Deactivate(true);
}

void ALaserActor::TrackMeteors()
{
	if (!bIsOnCooldown)
	{
		float LazerZLocation = GetActorLocation().Z;
		for (TActorIterator<AMeteorActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			AMeteorActor* Meteor = *ActorItr;
			float MeteorZLocation = Meteor->GetActorLocation().Z;
			float DeltaZ = LazerZLocation - MeteorZLocation;
			if (FMath::Abs(DeltaZ) < 20)
			{
				if (LaserBeamActorClass)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;

					FTransform SpawnTransform = GetActorTransform();

					FRotator Rotation = GetActorRotation();
					ALaserBeamActor* LaserBeam = GetWorld()->SpawnActor<ALaserBeamActor>(LaserBeamActorClass, SpawnTransform, SpawnParams);

					bIsOnCooldown = true;

					GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ALaserActor::ResetCooldown, 0.3f, false);
				}
			}
		}
	}
}

void ALaserActor::ResetCooldown()
{
	bIsOnCooldown = false;
}

void ALaserActor::Activate()
{
	bIsActivated = true;
	float StableDuration = DefaultDuration - ExpirationDuration;
	GetWorld()->GetTimerManager().SetTimer(LaserDurationHandle, this, &ALaserActor::OnExpire, StableDuration, false);
}

void ALaserActor::Deactivate(bool ShouldScheduleNextCollectable)
{
	if (ShouldScheduleNextCollectable)
	{
		if (ACollectablesController* CollectablesController = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetCollectablesController())
		{
			CollectablesController->ScheduleNextCollectable();
		}
	}

	this->Destroy();

}
