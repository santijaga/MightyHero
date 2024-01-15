#include "MeteorActor.h"
#include "PaperFlipbookComponent.h"

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

void AMeteorActor::SetFallVelocity(double NewVelocity)
{
	FallVelocity = NewVelocity;
}

void AMeteorActor::Destruction()
{
	bIsDestroyed = true;

	UPaperFlipbookComponent* RenderComponentRef = GetRenderComponent();
	RenderComponentRef->SetFlipbook(DestructionFlipbook);
	RenderComponentRef->SetLooping(false);

	if (!RenderComponentRef->OnFinishedPlaying.IsAlreadyBound(this, &AMeteorActor::OnFlipbookFinishedPlaying))
	{
		RenderComponentRef->OnFinishedPlaying.AddDynamic(this, &AMeteorActor::OnFlipbookFinishedPlaying);
	}
}

void AMeteorActor::OnFlipbookFinishedPlaying()
{
	this->Destroy();
}
