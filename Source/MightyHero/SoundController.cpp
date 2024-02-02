


#include "SoundController.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASoundController::ASoundController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
}

// Called when the game starts or when spawned
void ASoundController::BeginPlay()
{
	Super::BeginPlay();
	
	SetupBGM();
}

// Called every frame
void ASoundController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoundController::SetupBGM()
{
	BGMAudioComponent->bAutoActivate = false; // Не воспроизводить звук сразу же
	if (BGMAudioComponent != nullptr && BGMCue != nullptr)
	{
		BGMAudioComponent->SetSound(BGMCue);
	}
}

void ASoundController::SetSound(bool bSoundState)
{
	bIsSoundOn = bSoundState;

	if (bSoundState)
	{
		PlayBGM();
	}
	else
	{
		StopBGM();
	}
}

void ASoundController::PlayExplosion()
{
	if (ExplosionCue != nullptr && bIsSoundOn)
	{
		UGameplayStatics::PlaySound2D(this, ExplosionCue);
	}
}

void ASoundController::PlayJump()
{
	if (JumpCue != nullptr && bIsSoundOn)
	{
		UGameplayStatics::PlaySound2D(this, JumpCue);
	}
}

void ASoundController::PlayDestruction()
{
	if (DestructionCue != nullptr && bIsSoundOn)
	{
		UGameplayStatics::PlaySound2D(this, DestructionCue);
	}
}

void ASoundController::PlayShortCollectCue()
{
	if (bIsSoundOn && ShortCollectCue != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, ShortCollectCue);
	}
}

void ASoundController::PlayLongCollectCue()
{
	if (bIsSoundOn && LongCollectCue != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, LongCollectCue);
	}
}


void ASoundController::PlayBGM()
{
	if (BGMAudioComponent != nullptr && !BGMAudioComponent->IsPlaying() && bIsSoundOn)
	{
		BGMAudioComponent->Play();
	}
}

void ASoundController::StopBGM()
{
	if (BGMAudioComponent && BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->Stop();
	}
}

