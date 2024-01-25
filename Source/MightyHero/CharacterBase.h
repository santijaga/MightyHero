

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "CharacterBase.generated.h"

UENUM(BlueprintType)
enum class ECharacterStates : uint8 {
	SE_Idle = 0 UMETA(DisplayName="Idle"),
	SE_Fall = 1 UMETA(DisplayName = "Fall"),
	SE_Rise = 2 UMETA(DisplayName = "Rise"),
	SE_Impact = 3 UMETA(DisplayName = "Impact"),
	SE_Sad = 4 UMETA(DisplayName = "Sad"),
};

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float DefaultForwardSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float DefaultJumpVelocity = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	float DefaultGravityScale = 1.5f;
	
private:
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
	void EarthDestroyed();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void Fall();

private:
	UFUNCTION(Category = Movement)
	void Stop();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	ECharacterStates StatesEnum;

	UFUNCTION(BlueprintCallable, Category = State)
	void OnImpactEnded();

private:
	bool IsFreelyMoving();

public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void ResetCharacter();

public:
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void IncreaseDifficulty();

private:
	UPROPERTY()
	float difficultyLevel = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	TArray<float> JumpVelocityPerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	TArray<float> GravityScalePerLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bCrashedInSatellite = false;
};
