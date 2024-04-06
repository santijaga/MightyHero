

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "RPGPawn.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class ERPGCharacterGameStates : uint8 {
	SE_StandBy = 0 UMETA(DisplayName = "On stand by"),
	SE_Playing = 1 UMETA(DisplayName = "Active Gameplay")
};

UENUM(BlueprintType)
enum class ERPGCharacterAnimationStates : uint8 {
	SE_Anim_Rise = 0 UMETA(DisplayName = "Animation Rise"),
	SE_Anim_Fall = 1 UMETA(DisplayName = "Animation Fall"),
	SE_Anim_Aim = 2 UMETA(DisplayName = "Animation Aim"),
	SE_Anim_Idle = 3 UMETA(DisplayName = "Animation Idle"),
	SE_Anim_Attack = 4 UMETA(DisplayName = "Animation Attack"),
};

/**
 * 
 */
UCLASS()
class MIGHTYHERO_API ARPGPawn : public APaperCharacter
{
	GENERATED_BODY()

	ARPGPawn();
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> AimFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> AttackFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> FlyFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> RiseFlipbook;

	/*
	* Properties
	*/
private:
	ERPGCharacterAnimationStates PawnAnimState;
	ERPGCharacterGameStates PawnGameplayState;
	float StartPoint;
	bool bIsAiming = false;
	bool bIsAttacking = false;
	bool bIsFalling = false;
	bool bIsJumping = false;
	bool bIsIdle = false;
	bool bIsRising = false;

	/*
	* Interface
	*/
public:
	UFUNCTION(BlueprintCallable, Category=Behaviour)
	void GameOver();

	UFUNCTION(BlueprintCallable, Category=Data)
	float GetDistance();

	UFUNCTION(BlueprintCallable, Category=Behaviour)
	void StartGame();

	/*
	* Movement
	*/
private:
	void Fly();
	void Stay();

	/*
	* Animation
	*/
private:
	void CalculateAnimationState();
	void SelectFlipbook();

	/*
	* Utils
	*/
private:
	float CalculateDistance();
	bool IsGameOver();
	void SetFlipbook(UPaperFlipbookComponent* FlipbookComponent, TObjectPtr<UPaperFlipbook> Flipbook, bool Looping);

	/*
	* Collisions
	*/
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* MeeleHitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* ThreatCollider;

	/*
	* Timer Handles
	*/
private:
	FTimerHandle AimHandle;

	/*
	* EventHandlers
	*/
public:
	UFUNCTION()
	void OnAttackEnded();

	UFUNCTION()
	void OnMeeleHitBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Event Handler")
	void OnThreatColliderOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
