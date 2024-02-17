


#include "LaserToken.h"

void ALaserToken::Collect()
{
	Super::Collect();

	UE_LOG(LogTemp, Warning, TEXT("Child collect method was called"));
}