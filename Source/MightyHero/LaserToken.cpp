


#include "LaserToken.h"
#include "LaserActor.h"
#include "Kismet/GameplayStatics.h"

void ALaserToken::Collect()
{
	Super::Collect();

	if (LaserActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FTransform SpawnTransform = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorTransform();

		FRotator Rotation = GetActorRotation();
		ALaserActor* LaserActor = GetWorld()->SpawnActor<ALaserActor>(LaserActorClass, SpawnTransform, SpawnParams);

		LaserActor->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Laser Actor Class not specified"));
	}
}