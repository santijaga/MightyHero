


#include "CollectableBase.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "MightyHeroGameModeBase.h"
#include "SoundController.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterBase.h"

ACollectableBase::ACollectableBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SetupIdleFlipbook();
}

void ACollectableBase::Collect()
{
	UE_LOG(LogTemp, Warning, TEXT("Collect started!"));
	if (!bIsCollected)
	{
		UE_LOG(LogTemp, Warning, TEXT("Collectable not collected!"));

		bIsCollected = true;
		UE_LOG(LogTemp, Warning, TEXT("bIsCollected set!"));

		if (UPaperFlipbookComponent* RenderComponentRef = GetRenderComponent())
		{
			UE_LOG(LogTemp, Warning, TEXT("Got RenderComponentRef!"));

			RenderComponentRef->SetFlipbook(CollectedFlipbook);
			UE_LOG(LogTemp, Warning, TEXT("CollectedFlipbook set!"));

			RenderComponentRef->SetLooping(false);
			UE_LOG(LogTemp, Warning, TEXT("Looping setup!"));

			RenderComponentRef->PlayFromStart();
			UE_LOG(LogTemp, Warning, TEXT("Flipbook set to begging!"));

			if (!RenderComponentRef->OnFinishedPlaying.IsAlreadyBound(this, &ACollectableBase::DestroyCollectable))
			{
				RenderComponentRef->OnFinishedPlaying.AddDynamic(this, &ACollectableBase::DestroyCollectable);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Render Component Reference missing!"));
		}

		if (UBoxComponent* HitBox = FindComponentByClass<UBoxComponent>())
		{
			HitBox->DestroyComponent();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Hit Box Component Reference missing!"));
		}

		if (UWorld* GameWorld = GetWorld())
		{
			if (AMightyHeroGameModeBase* GameMode = Cast<AMightyHeroGameModeBase>(GameWorld->GetAuthGameMode()))
			{
				if (ASoundController* SoundController = GameMode->GetSoundController())
				{
					SoundController->PlayLongCollectCue();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Sound Component Reference missing!"));
				}
			}
		}
	}
}

void ACollectableBase::DestroyCollectable()
{
	this->Destroy();
}

void ACollectableBase::SetupIdleFlipbook()
{
	if (UPaperFlipbookComponent* RenderComponentRef = GetRenderComponent())
	{
		RenderComponentRef->SetFlipbook(IdleFlipbook);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Render Component Reference missing!"));
	}
}
