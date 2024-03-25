

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundController.generated.h"

class USoundCue;

UCLASS()
class MIGHTYHERO_API ASoundController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
	USoundCue* BGMCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundCue* JumpCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundCue* ExplosionCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundCue* DestructionCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundCue* ShortCollectCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundCue* LongCollectCue;

private:
	bool bIsSoundOn = true;
	class UAudioComponent* BGMAudioComponent;
	void PlayBGM();
	void StopBGM();
	void SetupBGM();

public:
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void SetSound(bool bSoundState);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayExplosion();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayJump(USoundCue* InJumpCue);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayDestruction();

	UFUNCTION(BlueprintCallable, Category = "SFX")
	void PlayShortCollectCue();

	UFUNCTION(BlueprintCallable, Category = "SFX")
	void PlayLongCollectCue();
};
