

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "ParticleFlipbookActor.generated.h"

class UPaperFlipbook;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AParticleFlipbookActor : public APaperFlipbookActor
{
	GENERATED_BODY()

public:
	AParticleFlipbookActor();

	virtual void BeginPlay() override;
	
	/*
	* Flipbook
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> ParticlesFlipbook;

	/*
	* Event handler
	*/
private:
	UFUNCTION()
	void DestroyParticles();
};
