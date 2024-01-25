

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundController.generated.h"

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
	class USoundCue* BGMCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	class USoundCue* JumpCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	class USoundCue* ExplosionCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	class USoundCue* DestructionCue;

private:
	bool bIsSoundOn = true;
	class UAudioComponent* BGMAudioComponent;

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayBGM();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void StopBGM();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void SetupBGM();

public:
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void SetSound(bool bSoundState);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayExplosion();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayJump();

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayDestruction();
};
