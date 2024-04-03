

#pragma once

#include "CoreMinimal.h"
#include "Missions/MissionActor.h"
#include "MissionOne.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMissionOne : public AMissionActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

};
