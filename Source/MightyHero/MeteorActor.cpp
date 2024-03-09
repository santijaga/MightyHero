#include "MeteorActor.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "MightyHeroGameModeBase.h"
#include "SoundController.h"
#include "CoinsController.h"

AMeteorActor::AMeteorActor()
{
	UPaperFlipbookComponent* Flipbook = GetRenderComponent();
	Flipbook->SetEnableGravity(false);

	PrimaryActorTick.bCanEverTick = true;
}

void AMeteorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsDestroyed)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Z -= FallVelocity * DeltaTime;
		SetActorLocation(NewLocation);
	}
}

void AMeteorActor::SetFallVelocity(float NewVelocity)
{
	FallVelocity = NewVelocity;
}

void AMeteorActor::Destruction()
{
	bIsDestroyed = true;

	UPaperFlipbookComponent* RenderComponentRef = GetRenderComponent();
	RenderComponentRef->SetFlipbook(DestructionFlipbook);
	RenderComponentRef->SetLooping(false);

	UBoxComponent* BoxComponent = FindComponentByClass<UBoxComponent>();

	if (BoxComponent)
	{
		BoxComponent->DestroyComponent();
	}

	if (ASoundController* SoundController = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetSoundController())
	{
		SoundController->PlayDestruction();
	}
	
	if (!RenderComponentRef->OnFinishedPlaying.IsAlreadyBound(this, &AMeteorActor::OnFlipbookFinishedPlaying))
	{
		RenderComponentRef->OnFinishedPlaying.AddDynamic(this, &AMeteorActor::OnFlipbookFinishedPlaying);
	}
}

void AMeteorActor::Destruction(bool bShouldSpawnCoin)
{
	if (bShouldSpawnCoin)
	{
		if (ACoinsController* CoinsController = Cast<ACoinsController>(Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetCoinsController()))
		{
			CoinsController->SpawnCoin(GetActorLocation());
		}
	}

	this->Destruction();
}

void AMeteorActor::OnFlipbookFinishedPlaying()
{
	this->Destroy();
}
