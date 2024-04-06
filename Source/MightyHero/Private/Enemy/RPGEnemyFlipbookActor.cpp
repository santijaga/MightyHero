


#include "Enemy/RPGEnemyFlipbookActor.h"
#include "Kismet/GameplayStatics.h"

void ARPGEnemyFlipbookActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[%s] I was spawned!"), *this->GetName());
}

void ARPGEnemyFlipbookActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("[%s] I'm ticking!"));

	if (APawn* P = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		float DeltaLocation = GetActorLocation().X - P->GetActorLocation().X;
		if (DeltaLocation < -2000.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] I'm too far away! Self destruction!"));
			this->Destroy();
		}
	}
}