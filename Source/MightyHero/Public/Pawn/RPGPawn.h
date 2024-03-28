

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "RPGPawn.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;

UENUM(BlueprintType)
enum class ERPGCharacterGameState : uint8 {
	SE_StandBy = 0 UMETA(DisplayName = "On stand by"),
	SE_Playing = 1 UMETA(DisplayName = "Active Gameplay")
};

UENUM(BlueprintType)
enum class ERPGCharacterStates : uint8 {
	SE_Idle = 0 UMETA(DisplayName = "Idle"),
	SE_Fall = 1 UMETA(DisplayName = "Fall"),
	SE_Rise = 2 UMETA(DisplayName = "Rise"),
	SE_Strike = 3 UMETA(DisplayName = "Strike"),
	SE_Lose = 4 UMETA(DisplayName = "Lose"),
	SE_Aim = 5 UMETA(DisplayName = "Aim"),
	SE_Death = 6 UMETA(DisplayName = "Death"),
	SE_Jump = 7 UMETA(DisplayName = "Jump")
};

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ARPGPawn : public APaperCharacter
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/*
	* User input
	*/
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void TouchPressed(ETouchIndex::Type FingerIndex, FVector Location);

	/*
	* Settings
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	float BaseGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	float BaseFlySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	float BaseJumpVelocity;

	/*
	* Flipbooks
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> FlyFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> RiseFlipbook;

	/*
	* Properties
	*/
private:
	ERPGCharacterGameState PawnGameplayState;
	ERPGCharacterStates PawnState;

	/*
	* Interface
	*/
public:
	UFUNCTION(BlueprintCallable, Category=Behaviour)
	void StartGameplay();

	/*
	* Movement
	*/
private:
	void Fly();

	/*
	* Animation
	*/
private:
	void SelectFlipbook();
	void SetFlyFlipbook(UPaperFlipbookComponent* Flipbook);
	void SetRiseFlipbook(UPaperFlipbookComponent* Flipbook);
};
