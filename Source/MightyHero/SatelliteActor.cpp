#include "SatelliteActor.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "MightyHeroGameModeBase.h"
#include "Sound/SoundController.h"

ASatelliteActor::ASatelliteActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetupIdleFlipbook();
}

void ASatelliteActor::Destruction()
{
	bIsDestroyed = true;

	UPaperFlipbookComponent* RenderComponentRef = GetRenderComponent();
	RenderComponentRef->SetFlipbook(DestructionFlipbook);
	RenderComponentRef->SetLooping(false);
	RenderComponentRef->PlayFromStart();
	RemoveAllSubs(RenderComponentRef);

	UBoxComponent* BoxComponent = FindComponentByClass<UBoxComponent>();

	if (BoxComponent)
	{
		BoxComponent->DestroyComponent();
	}

	if (ASoundController* SoundController = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetSoundController())
	{
		SoundController->PlayExplosion();
	}

	if (!RenderComponentRef->OnFinishedPlaying.IsAlreadyBound(this, &ASatelliteActor::DestroySatellite))
	{
		RenderComponentRef->OnFinishedPlaying.AddDynamic(this, &ASatelliteActor::DestroySatellite);
	}
}

void ASatelliteActor::SetupIdleFlipbook()
{
	UPaperFlipbookComponent* RenderComponentRef = GetRenderComponent();
	RenderComponentRef->SetEnableGravity(false);
	RenderComponentRef->SetLooping(false);
	RenderComponentRef->SetFlipbook(IdleFlipbook);
	RenderComponentRef->PlayFromStart();
	
	RemoveAllSubs(RenderComponentRef);

	RenderComponentRef->OnFinishedPlaying.AddDynamic(this, &ASatelliteActor::OnIdleFlipbookFinishedPlaying);
}

void ASatelliteActor::OnIdleFlipbookFinishedPlaying()
{
	if (repeatsTillSmoke > 0)
	{
		repeatsTillSmoke--;
		SetupIdleFlipbook();
	}
	else
	{
		UPaperFlipbookComponent* RenderComponentRef = GetRenderComponent();
		RenderComponentRef->SetEnableGravity(false);
		RenderComponentRef->SetFlipbook(SmokeFlipbook);
		RenderComponentRef->SetLooping(false);
		RenderComponentRef->PlayFromStart();

		RemoveAllSubs(RenderComponentRef);
		RenderComponentRef->OnFinishedPlaying.AddDynamic(this, &ASatelliteActor::OnSmokeFlipbookFinishedPlaying);
	}
}

void ASatelliteActor::DestroySatellite()
{
	this->Destroy();
}

void ASatelliteActor::OnSmokeFlipbookFinishedPlaying()
{
	repeatsTillSmoke = 3;
	SetupIdleFlipbook();
}

void ASatelliteActor::RemoveAllSubs(UPaperFlipbookComponent* RenderComponentRef)
{
	RenderComponentRef->OnFinishedPlaying.RemoveAll(this);
}
