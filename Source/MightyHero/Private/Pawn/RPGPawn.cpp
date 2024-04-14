


#include "Pawn/RPGPawn.h"

#include "Core/MightyHeroRPGGameModeBase.h"
#include "Core/MightyHeroPlayerController.h"
#include "Core/MightyHeroPlayerState.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Enemy/RPGEnemyFlipbookActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "Particles/ParticleFlipbookActor.h"
#include "Projectile/ProjectileFlipbookActor.h"

ARPGPawn::ARPGPawn()
{
	ThreatCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Threat Collider"));
	ThreatCollider->SetupAttachment(RootComponent);

	MeeleHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Meele Hit Box"));
	MeeleHitBox->SetupAttachment(RootComponent);

	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Fire Point"));
	FirePoint->SetupAttachment(RootComponent);
}

void ARPGPawn::BeginPlay()
{
	Super::BeginPlay();

	if (AMightyHeroPlayerController* PC = Cast<AMightyHeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(GameplayMappingContext, 0);
		}
	}

	ThreatCollider->OnComponentBeginOverlap.AddDynamic(this, &ARPGPawn::OnThreatColliderOverlapBegin);
	MeeleHitBox->OnComponentBeginOverlap.AddDynamic(this, &ARPGPawn::OnMeeleHitBoxOverlapBegin);

	bIsIdle = true;
	PawnGameplayState = ERPGCharacterGameStates::SE_StandBy;
	StartPoint = GetActorLocation().X;
}

void ARPGPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PawnGameplayState == ERPGCharacterGameStates::SE_Playing)
	{
		Fly();

		if (IsGameOver())
		{
			if (AMightyHeroRPGGameModeBase* GM = Cast<AMightyHeroRPGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				GM->GameOver();
			}
		}
	}

	if (PawnGameplayState == ERPGCharacterGameStates::SE_StandBy)
	{
		Stay();
	}

	CalculateAnimationState();
	SelectFlipbook();
}

/*
* User input
*/
void ARPGPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARPGPawn::TouchPressed);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ARPGPawn::Jump);
		EIC->BindAction(RiseUpAction, ETriggerEvent::Triggered, this, &ARPGPawn::RiseUp);
		EIC->BindAction(ShotAction, ETriggerEvent::Started, this, &ARPGPawn::Shot);
	}
}

void ARPGPawn::RiseUpActionTriggered(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		RiseUp();
	}
}

void ARPGPawn::ShotActionTriggered(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Shot();
	}
}

void ARPGPawn::TouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	RiseUp();
}

/*
* Interface
*/
void ARPGPawn::GameOver()
{
	PawnGameplayState = ERPGCharacterGameStates::SE_StandBy;
}

float ARPGPawn::GetDistance()
{
	return CalculateDistance();
}

void ARPGPawn::TakeHit(float Damage)
{
	if (AMightyHeroPlayerState* PS = Cast<AMightyHeroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
	{
		PS->ApplyDamage(Damage);
	}
}

void ARPGPawn::StartGame()
{
	PawnGameplayState = ERPGCharacterGameStates::SE_Playing;
	bIsIdle = false;
}

/*
* Movement
*/
void ARPGPawn::Fly()
{
	if (UCharacterMovementComponent* MC = GetCharacterMovement())
	{
		MC->GravityScale = BaseGravityScale;
		FVector CurrentVelocity = MC->Velocity;
		FVector ForwardDirection = GetActorForwardVector();
		FVector NewVelocity = ForwardDirection * BaseFlySpeed;
		if (bIsRisingUp)
		{
			NewVelocity.Z = BaseRiseUpVelocity;
			bIsRisingUp = false;
		}
		else
		{
			NewVelocity.Z = CurrentVelocity.Z;
		}

		if (NewVelocity.Z > 0)
		{
			bIsRising = true;
			bIsFalling = false;
		}

		if (NewVelocity.Z < 0)
		{
			bIsFalling = true;
			bIsRising = false;
		}

		MC->Velocity = NewVelocity;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Movement component not found"), *this->GetName());
	}
}

void ARPGPawn::RiseUp()
{
	if (PawnGameplayState == ERPGCharacterGameStates::SE_Playing)
	{
		bIsRisingUp = true;
	}
}

void ARPGPawn::Stay()
{
	if (UCharacterMovementComponent* MC = GetCharacterMovement())
	{
		MC->GravityScale = 0;
		MC->Velocity = FVector(0, 0, 0);
	}
}

/*
* Actions
*/
void ARPGPawn::Attack(AActor* OtherActor)
{
	if (ARPGEnemyFlipbookActor* Enemy = Cast<ARPGEnemyFlipbookActor>(OtherActor))
	{
		bIsAttacking = true;
		bIsAiming = false;
		GetWorld()->GetTimerManager().ClearTimer(AimHandle);
		int32 Strength = Cast<AMightyHeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetStrength();
		float Damage = Strength * 10;
		Enemy->GetHit(Damage);
		GetWorld()->SpawnActor<AParticleFlipbookActor>(AttackParticles, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
	}
}

void ARPGPawn::Shot()
{
	if (bIsShootingAlowed)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s shot!"), *this->GetName());
		bIsShooting = true;
		bIsShootingAlowed = false;
		float GunCooldown = DefaultGunCooldown;
		if (UWorld* W = GetWorld())
		{
			W->GetTimerManager().SetTimer(GunCooldownHandle, [this]()
				{
					bIsShootingAlowed = true;
				}, GunCooldown, false);

			if (FirePoint)
			{
				W->SpawnActor<AParticleFlipbookActor>(ShotParticles, FirePoint->GetComponentLocation(), FirePoint->GetComponentRotation());
				
				// spawn projectile passing damage to constructor AProjectileFlipbookActor(float Damage);
				float Damage = 10.0f;
				AProjectileFlipbookActor* SpawnedProjectile = W->SpawnActor<AProjectileFlipbookActor>(FirePoint->GetComponentLocation(), FirePoint->GetComponentRotation(), FActorSpawnParameters());
				if (SpawnedProjectile)
				{
					SpawnedProjectile->SetDamage(Damage);
				}
			}
		}
	}
}

/*
* Animation
*/
void ARPGPawn::CalculateAnimationState()
{
	if (bIsIdle)
	{
		PawnAnimState = ERPGCharacterAnimationStates::SE_Anim_Idle;
	}
	else
	{
		if (bIsAttacking)
		{
			PawnAnimState = ERPGCharacterAnimationStates::SE_Anim_Attack;
		}
		else
		{
			if (bIsAiming)
			{
				PawnAnimState = ERPGCharacterAnimationStates::SE_Anim_Aim;
			}
			else
			{
				if (bIsShooting)
				{
					PawnAnimState = ERPGCharacterAnimationStates::SE_Anim_Shot;
				}
				else
				{
					if (bIsGunAiming)
					{
						PawnAnimState = ERPGCharacterAnimationStates::SE_Anim_GunAim;
					}
					else
					{
						if (bIsGunAimEnd)
						{
							PawnAnimState = ERPGCharacterAnimationStates::SE_Anim_GunAimEnd;
						}
						else
						{
							if (bIsRising)
							{
								PawnAnimState = ERPGCharacterAnimationStates::SE_Anim_Rise;
							}

							if (bIsFalling)
							{
								PawnAnimState = ERPGCharacterAnimationStates::SE_Anim_Fall;
							}
						}
					}
				}
			}
		}
	}
}

void ARPGPawn::SelectFlipbook()
{
	if (UPaperFlipbookComponent* Flipbook = GetSprite())
	{
		if (PawnAnimState == ERPGCharacterAnimationStates::SE_Anim_Attack)
		{
			if (Flipbook->GetFlipbook() != AttackFlipbook)
			{
				SetFlipbook(Flipbook, AttackFlipbook, false);
				Flipbook->OnFinishedPlaying.AddDynamic(this, &ARPGPawn::OnAttackEnded);
			}
		}

		if (PawnAnimState == ERPGCharacterAnimationStates::SE_Anim_Idle)
		{
			if (Flipbook->GetFlipbook() != IdleFlipbook)
			{
				SetFlipbook(Flipbook, IdleFlipbook, true);
			}
		}

		if (PawnAnimState == ERPGCharacterAnimationStates::SE_Anim_Aim)
		{
			if (Flipbook->GetFlipbook() != AimFlipbook)
			{
				SetFlipbook(Flipbook, AimFlipbook, true);
			}
		}

		if (PawnAnimState == ERPGCharacterAnimationStates::SE_Anim_Fall)
		{
			if (Flipbook->GetFlipbook() != FlyFlipbook)
			{
				SetFlipbook(Flipbook, FlyFlipbook, true);
			}
		}

		if (PawnAnimState == ERPGCharacterAnimationStates::SE_Anim_Rise)
		{
			if (Flipbook->GetFlipbook() != RiseFlipbook)
			{
				SetFlipbook(Flipbook, RiseFlipbook, true);
			}
		}

		if (PawnAnimState == ERPGCharacterAnimationStates::SE_Anim_Shot)
		{
			if (Flipbook->GetFlipbook() != ShotFlipbook)
			{
				SetFlipbook(Flipbook, ShotFlipbook, false);
				Flipbook->OnFinishedPlaying.AddDynamic(this, &ARPGPawn::OnShotEnded);
			}
		}

		if (PawnAnimState == ERPGCharacterAnimationStates::SE_Anim_GunAim)
		{
			if (Flipbook->GetFlipbook() != GunAimFlipbook)
			{
				SetFlipbook(Flipbook, GunAimFlipbook, false);
				Flipbook->OnFinishedPlaying.AddDynamic(this, &ARPGPawn::OnAimEnded);
			}
		}

		if (PawnAnimState == ERPGCharacterAnimationStates::SE_Anim_GunAimEnd)
		{
			if (Flipbook->GetFlipbook() != GunAimEndFlipbook)
			{
				SetFlipbook(Flipbook, GunAimEndFlipbook, false);
				Flipbook->OnFinishedPlaying.AddDynamic(this, &ARPGPawn::OnGunDisapear);
			}
		}
	}
}

/*
* Utils
*/
float ARPGPawn::CalculateDistance()
{
	return GetActorLocation().X - StartPoint;
}

bool ARPGPawn::IsGameOver()
{
	if (AMightyHeroRPGGameModeBase* GM = Cast<AMightyHeroRPGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		FVector CurrentLocation = GetActorLocation();
		if (CurrentLocation.Z < GM->BottomBound)
		{
			return true;
		}

		if (CurrentLocation.Z > GM->TopBound)
		{
			return true;
		}
	}

	return false;
}

void ARPGPawn::SetFlipbook(UPaperFlipbookComponent* FlipbookComponent, TObjectPtr<UPaperFlipbook> Flipbook, bool Looping)
{
	FlipbookComponent->SetFlipbook(Flipbook);
	FlipbookComponent->SetLooping(Looping);
	FlipbookComponent->PlayFromStart();
}

/*
* EventHandlers
*/
void ARPGPawn::OnAimEnded()
{
	bIsGunAiming = false;
	bIsGunAimEnd = true;

	if (UPaperFlipbookComponent* Flipbook = GetSprite())
	{
		Flipbook->OnFinishedPlaying.RemoveAll(this);
	}
}

void ARPGPawn::OnAttackEnded()
{
	bIsAttacking = false;

	if (UPaperFlipbookComponent* Flipbook = GetSprite())
	{
		Flipbook->OnFinishedPlaying.RemoveAll(this);
	}
}

void ARPGPawn::OnGunDisapear()
{
	bIsGunAimEnd = false;

	if (UPaperFlipbookComponent* Flipbook = GetSprite())
	{
		Flipbook->OnFinishedPlaying.RemoveAll(this);
	}
}

void ARPGPawn::OnMeeleHitBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Attack(OtherActor);
}

void ARPGPawn::OnShotEnded()
{
	bIsShooting = false;
	bIsGunAiming = true;

	if (UPaperFlipbookComponent* Flipbook = GetSprite())
	{
		Flipbook->OnFinishedPlaying.RemoveAll(this);
	}
}

void ARPGPawn::OnThreatColliderOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARPGEnemyFlipbookActor* Enemy = Cast<ARPGEnemyFlipbookActor>(OtherActor))
	{
		bIsAiming = true;
		GetWorld()->GetTimerManager().SetTimer(AimHandle, [this]() mutable
			{
				bIsAiming = false;
				GetWorld()->GetTimerManager().ClearTimer(AimHandle);
			}, 2.0f, false);
	}
}