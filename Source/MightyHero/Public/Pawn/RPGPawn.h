

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InputActionValue.h"
#include "RPGPawn.generated.h"

class AParticleFlipbookActor;
class AProjectileFlipbookActor;
class UInputMappingContext;
class UInputAction;
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
	SE_Anim_Shot = 5 UMETA(DisplayName = "Animation Shot"),
	SE_Anim_GunAim = 6 UMETA(DisplayName = "Animation Gun Aim"),
	SE_Anim_GunAimEnd = 7 UMETA(DisplayName = "Animation Gun Aim End"),
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="User input")
	UInputMappingContext* GameplayMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User input")
	UInputAction* JumpAction;
	UInputAction* RiseUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User input")
	UInputAction* ShotAction;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void RiseUpActionTriggered(const FInputActionValue& Value);
	void ShotActionTriggered(const FInputActionValue& Value);
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
	float BaseRiseUpVelocity;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> ShotFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> GunAimEndFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flipbooks, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> GunAimFlipbook;

	/*
	* Properties
	*/
private:
	bool bIsAiming = false;
	bool bIsAttacking = false;
	bool bIsGunAiming = false;
	bool bIsGunAimEnd = false;
	bool bIsFalling = false;
	bool bIsJumping = false;
	bool bIsRisingUp = false;
	bool bIsIdle = false;
	bool bIsRising = false;
	bool bIsShooting = false;
	bool bIsShootingAlowed = true;
	float DefaultGunCooldown = 1.f;
	ERPGCharacterAnimationStates PawnAnimState;
	ERPGCharacterGameStates PawnGameplayState;
	float StartPoint;

	/*
	* Interface
	*/
public:
	UFUNCTION(BlueprintCallable, Category=Behaviour)
	void GameOver();

	UFUNCTION(BlueprintCallable, Category=Data)
	float GetDistance();

	void TakeHit(float Damage);

	UFUNCTION(BlueprintCallable, Category=Behaviour)
	void StartGame();

	/*
	* Movement
	*/
private:
	void Fly();
	void RiseUp();
	void Stay();

	/*
	* Actions
	*/
private:
	void Attack(AActor* OtherActor);
	void Shot();

	/*
	* Animation
	*/
private:
	void CalculateAnimationState();
	void SelectFlipbook();

	/*
	* SFX
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SFX", meta = (DisplayThumbnail = "true"))
	TSubclassOf<AParticleFlipbookActor> AttackParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (DisplayThumbnail = "true"))
	TSubclassOf<AProjectileFlipbookActor> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX", meta = (DisplayThumbnail = "true"))
	TSubclassOf<AParticleFlipbookActor> ShotParticles;

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
	* Points
	*/
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Points")
	USceneComponent* FirePoint;

	/*
	* Timer Handles
	*/
private:
	FTimerHandle AimHandle;
	FTimerHandle GunCooldownHandle;

	/*
	* EventHandlers
	*/
public:
	UFUNCTION()
	void OnAimEnded();

	UFUNCTION()
	void OnAttackEnded();

	UFUNCTION()
	void OnGunDisapear();

	UFUNCTION()
	void OnMeeleHitBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnShotEnded();

	UFUNCTION(BlueprintCallable, Category = "Event Handler")
	void OnThreatColliderOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
