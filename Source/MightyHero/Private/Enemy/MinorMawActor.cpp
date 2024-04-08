


#include "Enemy/MinorMawActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "Pawn/RPGPawn.h"

AMinorMawActor::AMinorMawActor()
{
	MeeleHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Meele Hit Box"));
	MeeleHitBox->SetupAttachment(RootComponent);
}

void AMinorMawActor::BeginPlay()
{
	Super::BeginPlay();

	HP = 10;

	if (UPaperFlipbookComponent* Flipbook = GetRenderComponent())
	{
		SetFlipbook(Flipbook, FlyFlipbook, true);
	}

	MeeleHitBox->OnComponentBeginOverlap.AddDynamic(this, &AMinorMawActor::OnMeeleHitBoxOverlapBegin);
}

void AMinorMawActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
* Event Handlers
*/
void AMinorMawActor::OnBiteFinish()
{
	if (UPaperFlipbookComponent* Flipbook = GetRenderComponent())
	{
		if (Flipbook->GetFlipbook() != FlyFlipbook)
		{
			SetFlipbook(Flipbook, FlyFlipbook, true);
		}
	}

}

void AMinorMawActor::OnDeath()
{
	if (UPaperFlipbookComponent* Flipbook = GetRenderComponent())
	{
		if (Flipbook->GetFlipbook() != DeathFlipbook)
		{
			SetFlipbook(Flipbook, DeathFlipbook, false);
		}
	}
}

void AMinorMawActor::OnMeeleHitBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARPGPawn* P = Cast<ARPGPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		P->TakeHit(BiteDamage);

		if (UPaperFlipbookComponent* Flipbook = GetRenderComponent())
		{
			if (Flipbook->GetFlipbook() != BiteFlipbook)
			{
				SetFlipbook(Flipbook, BiteFlipbook, false);

				Flipbook->OnFinishedPlaying.AddDynamic(this, &AMinorMawActor::OnBiteFinish);
			}
		}
	}
}