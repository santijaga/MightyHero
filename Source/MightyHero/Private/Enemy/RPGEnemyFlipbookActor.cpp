


#include "Enemy/RPGEnemyFlipbookActor.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"

ARPGEnemyFlipbookActor::ARPGEnemyFlipbookActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARPGEnemyFlipbookActor::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGEnemyFlipbookActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TrackPushBack();
	TrackFlyAway();
	TrackDeath();
}

/*
* Interface
*/
void ARPGEnemyFlipbookActor::GetHit(float Damage)
{
	HP -= Damage;
	if (HP > 0)
	{
		bIsBeingPushed = true;
		PushStartLocation = GetActorLocation();
		PushTargetLocation = FVector(PushStartLocation.X + PushBackDistance, PushStartLocation.Y, PushStartLocation.Z);
		PushStartTime = GetWorld()->GetTimeSeconds();
	}
}

/*
* Trackers
*/
bool ARPGEnemyFlipbookActor::TrackDeath()
{
	if (HP <= 0)
	{
		OnDeath();
		return true;
	}

	return false;
}

void ARPGEnemyFlipbookActor::TrackFlyAway()
{
	if (APawn* P = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		float DeltaLocation = GetActorLocation().X - P->GetActorLocation().X;
		if (DeltaLocation < -2000.f)
		{
			this->Destroy();
		}
	}
}

void ARPGEnemyFlipbookActor::TrackPushBack()
{
	if (bIsBeingPushed)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		float TimeSinceStart = CurrentTime - PushStartTime;
		if (TimeSinceStart < PushDuration)
		{
			float Alpha = TimeSinceStart / PushDuration;
			float EaseOutAlpha = FMath::InterpEaseOut(0.0f, 1.0f, Alpha, 5.0f);

			FVector NewLocation = FMath::Lerp(PushStartLocation, PushTargetLocation, EaseOutAlpha);
			SetActorLocation(NewLocation);
		}
		else
		{
			bIsBeingPushed = false;
		}
	}
}

/*
* Utils
*/
void ARPGEnemyFlipbookActor::SetFlipbook(UPaperFlipbookComponent* FlipbookComponent, TObjectPtr<UPaperFlipbook> Flipbook, bool Looping)
{
	FlipbookComponent->OnFinishedPlaying.RemoveAll(this);
	FlipbookComponent->SetFlipbook(Flipbook);
	FlipbookComponent->SetLooping(Looping);
	FlipbookComponent->PlayFromStart();
}

/*
* Event Handlers
*/
void ARPGEnemyFlipbookActor::OnDeath()
{
	this->Destroy();
}