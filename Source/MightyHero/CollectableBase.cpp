


#include "CollectableBase.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "MightyHeroGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterBase.h"

ACollectableBase::ACollectableBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SetupIdleFlipbook();
}

void ACollectableBase::Collect()
{
	if (!bIsCollected)
	{
		bIsCollected = true;
		
		if (UPaperFlipbookComponent* RenderComponentRef = GetRenderComponent())
		{
			RenderComponentRef->SetFlipbook(CollectedFlipbook);
			RenderComponentRef->SetLooping(false);
			RenderComponentRef->PlayFromStart();
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
