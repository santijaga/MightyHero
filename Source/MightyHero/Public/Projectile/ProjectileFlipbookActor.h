

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "ProjectileFlipbookActor.generated.h"

class AParticleFlipbookActor;
class UBoxComponent;
class UPaperFlipbook;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AProjectileFlipbookActor : public APaperFlipbookActor
{
	GENERATED_BODY()

public:
	AProjectileFlipbookActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/*
	* Accessors
	*/
public:
	float GetDamage() const { return Damage; }
	void SetDamage(float NewDamage) { Damage = NewDamage; }

	/*
	* Properties
	*/
private:
	float Damage = 10.0f;

	/*
	* Settings
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float Speed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float FlyAwayDistance = 1000.0f;
	
	/*
	* Flipbooks
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> ProjectileFlipbook;

	/*
	* Events
	*/
public:
	UFUNCTION()
	void OnHitTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*
	* Behaviour
	*/
private:
	void Move(float DeltaSeconds);
	void DestroySelf();
	void TrackForFlyAway();
	void SpawnFlyParticles();

	/*
	* Collision
	*/
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* HitBox;

	/*
	* FX
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX", meta = (DisplayThumbnail = "true"))
	TSubclassOf<AParticleFlipbookActor> FlyParticles;

	/*
	* Timer
	*/
private:
	FTimerHandle TimerHandle_FlyParticles;
};
