

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "CharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ACharacterBase : public APaperZDCharacter
{
	GENERATED_BODY()

private:
	float ForwardSpeed = .0f;
	float JumpVelocity = .0f;
	bool bWasJump = false;

	float DefaultForwardSpeed = 300.f;
	float DefaultJumpVelocity = 500.f;
	float DefaultGravityScale = 1.0f;
	bool bIsMovementAllowed = false;

	class UCharacterMovementComponent* MovementComponent;

	void TouchPressed(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void StartGameplay();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void CharacterJump();

private:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void EarthDestroed();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void Fall();

private:
	UFUNCTION(Category = Movement)
	void Stop();

public:

};
