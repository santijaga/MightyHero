


#include "AuraActor.h"
#include "CharacterBase.h"
#include "MeteorActor.h"
#include "MightyHeroPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "MightyHeroGameModeBase.h"
#include "CollectablesController.h"
#include "CollectableBase.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Sound/SoundController.h"
#include "Coin.h"

AAuraActor::AAuraActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAuraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TrackCharacter();
}

void AAuraActor::Activate()
{
	OnActorBeginOverlap.AddDynamic(this, &AAuraActor::OnOverlapBegin);
	float StableDuration = DefaultDuration - ExpirationDuration;
	GetWorld()->GetTimerManager().SetTimer(AuraDurationHandle, this, &AAuraActor::OnExpire, StableDuration, false);
}

void AAuraActor::Deactivate(bool ShouldScheduleNextCollectable)
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

void AAuraActor::TrackCharacter()
{
	if (UGameplayStatics::GetPlayerPawn(this, 0))
	{
		SetActorLocation(UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation());
	}
}

void AAuraActor::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	AMeteorActor* MeteorActor = Cast<AMeteorActor>(OtherActor);
	if (MeteorActor)
	{
		if (UGameplayStatics::GetPlayerPawn(this, 0))
		{
			UGameplayStatics::GetPlayerPawn(this, 0)->GetPlayerState<AMightyHeroPlayerState>()->AddScore();

			if (IsValid(MeteorActor))
			{
				MeteorActor->Destruction(true);
			}
		}
	}

	if (ACollectableBase* Collectable = Cast<ACollectableBase>(OtherActor))
	{
		if (ACoin* Coin = Cast<ACoin>(Collectable))
		{
			return;
		}

		if (ACollectablesController* CollectablesController = Cast<ACollectablesController>(Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetCollectablesController()))
		{
			CollectablesController->CollectableBehaviour(Collectable);
		}
	}
}

void AAuraActor::OnExpire()
{
	if (UPaperFlipbookComponent* Flipbook = GetRenderComponent())
	{
		Flipbook->SetFlipbook(ExpireFlipbook);
	}

	GetWorld()->GetTimerManager().SetTimer(AuraDurationHandle, this, &AAuraActor::OnDurationEnds, ExpirationDuration, false);
}

void AAuraActor::OnDurationEnds()
{
	if (ASoundController* SoundController = Cast<ASoundController>(Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetSoundController()))
	{
		SoundController->PlayLongCollectCue();
	}

	Deactivate(true);
}
