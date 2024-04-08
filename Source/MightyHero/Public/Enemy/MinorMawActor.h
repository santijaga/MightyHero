

#pragma once

#include "CoreMinimal.h"
#include "Enemy/RPGEnemyFlipbookActor.h"
#include "MinorMawActor.generated.h"

class UPaperFlipbook;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API AMinorMawActor : public ARPGEnemyFlipbookActor
{
	GENERATED_BODY()

public:
	AMinorMawActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
	* Properties
	*/
private:
	float BiteDamage = 1.f;

	/*
	* Collision
	*/
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* MeeleHitBox;

	/*
	* Flipbooks
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> BiteFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> DeathFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> FlyFlipbook;

	/*
	* Event Handlers
	*/
private:
	UFUNCTION()
	void OnBiteFinish();

	virtual void OnDeath() override;

	UFUNCTION()
	void OnMeeleHitBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
