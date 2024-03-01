

#pragma once

#include "CoreMinimal.h"
#include "CollectableBase.h"
#include "LaserToken.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ALaserToken : public ACollectableBase
{
	GENERATED_BODY()
	
public:
	void Collect() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<class ALaserActor> LaserActorClass;
};
