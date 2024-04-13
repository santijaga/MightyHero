


#include "Particles/ParticleFlipbookActor.h"

#include "PaperFlipbookComponent.h"

AParticleFlipbookActor::AParticleFlipbookActor()
{
	PrimaryActorTick.bCanEverTick = false;
	// disable collision
	GetRenderComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// disable overlap events
	GetRenderComponent()->SetGenerateOverlapEvents(false);

}

void AParticleFlipbookActor::BeginPlay()
{
	Super::BeginPlay();

	if (UPaperFlipbookComponent* Flipbook = GetRenderComponent())
	{
		Flipbook->SetFlipbook(ParticlesFlipbook);
		Flipbook->SetLooping(false);
		Flipbook->OnFinishedPlaying.AddDynamic(this, &AParticleFlipbookActor::DestroyParticles);
	}
}

void AParticleFlipbookActor::DestroyParticles()
{
	this->Destroy();
}