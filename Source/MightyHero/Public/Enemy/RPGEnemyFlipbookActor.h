

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "RPGEnemyFlipbookActor.generated.h"

class UPaperFlipbookComponent;
class UPaperFlipbook;

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ARPGEnemyFlipbookActor : public APaperFlipbookActor
{
	GENERATED_BODY()

public:
	ARPGEnemyFlipbookActor();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
	* Properties
	*/
protected:
	bool bIsBeingPushed = false;
	float HP = 0;
	float PushBackDistance = 600.f;
	float PushDuration = 0.5f;
	FVector PushStartLocation;
	float PushStartTime;
	FVector PushTargetLocation;

	/*
	* Interface
	*/
public:
	virtual void GetHit(float Damage);

	/*
	* Tracker
	*/
private:
	bool TrackDeath();
	void TrackFlyAway();
	void TrackPushBack();

	/*
	* Utils
	*/
protected:
	void SetFlipbook(UPaperFlipbookComponent* FlipbookComponent, TObjectPtr<UPaperFlipbook> Flipbook, bool Looping);

	/*
	* Event Handlers
	*/
	virtual void OnDeath();
};
