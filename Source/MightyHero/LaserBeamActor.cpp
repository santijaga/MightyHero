#include "LaserBeamActor.h"
#include "Kismet/GameplayStatics.h"
#include "MeteorActor.h"
#include "MightyHeroPlayerState.h"
#include "PaperFlipbookComponent.h"

ALaserBeamActor::ALaserBeamActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALaserBeamActor::BeginPlay()
{
	Super::BeginPlay();

	if (UPaperFlipbookComponent* RenderComponentRef = GetRenderComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *RenderComponentRef->GetName());
		RenderComponentRef->SetLooping(false);
		RenderComponentRef->SetFlipbook(IdleFlipbook);
		RenderComponentRef->PlayFromStart();
		RenderComponentRef->OnFinishedPlaying.AddDynamic(this, &ALaserBeamActor::DestroyLaser);
	}

	OnActorBeginOverlap.AddDynamic(this, &ALaserBeamActor::OnOverlapBegin);
}

void ALaserBeamActor::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	AMeteorActor* MeteorActor = Cast<AMeteorActor>(OtherActor);
	if (MeteorActor)
	{
		if (UGameplayStatics::GetPlayerPawn(this, 0))
		{
			Cast<AMightyHeroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0))->AddScore();

			if (IsValid(MeteorActor))
			{
				MeteorActor->Destruction(true);
			}
		}
	}
}

void ALaserBeamActor::DestroyLaser()
{
	UE_LOG(LogTemp, Warning, TEXT("Destroy Called for %s"), *this->GetName());
	this->Destroy();
}