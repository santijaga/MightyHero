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

	FVector NewLocation = GetActorLocation();
	NewLocation.Z -= FallVelocity * DeltaTime;
	SetActorLocation(NewLocation);
}

void AMeteorActor::SetFallVelocity(double NewVelocity)
{
	FallVelocity = NewVelocity;
}

void AMeteorActor::Destruction()
{
	this->Destroy();
}