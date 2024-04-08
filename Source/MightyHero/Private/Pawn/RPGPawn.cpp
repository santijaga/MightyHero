


#include "Pawn/RPGPawn.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Core/MightyHeroRPGGameModeBase.h"
#include "Core/MightyHeroPlayerController.h"
#include "Core/MightyHeroPlayerState.h"
#include "Components/BoxComponent.h"
#include "Enemy/RPGEnemyFlipbookActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"

ARPGPawn::ARPGPawn()
{
	ThreatCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Threat Collider"));
	ThreatCollider->SetupAttachment(RootComponent);

	MeeleHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Meele Hit Box"));
	MeeleHitBox->SetupAttachment(RootComponent);
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
	}
}

void ARPGPawn::JumpActionTriggered(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump();
	}
}

void ARPGPawn::TouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
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
		if (bIsJumping)
		{
			NewVelocity.Z = BaseJumpVelocity;
			bIsJumping = false;
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

void ARPGPawn::Jump()
{
	if (PawnGameplayState == ERPGCharacterGameStates::SE_Playing)
	{
		bIsJumping = true;
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
void ARPGPawn::OnAttackEnded()
{
	bIsAttacking = false;

	if (UPaperFlipbookComponent* Flipbook = GetSprite())
	{
		Flipbook->OnFinishedPlaying.RemoveAll(this);
	}
}

void ARPGPawn::OnMeeleHitBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARPGEnemyFlipbookActor* Enemy = Cast<ARPGEnemyFlipbookActor>(OtherActor))
	{
		bIsAttacking = true;
		bIsAiming = false;
		GetWorld()->GetTimerManager().ClearTimer(AimHandle);
		int32 Strength = Cast<AMightyHeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetStrength();
		float Damage = Strength * 10;
		Enemy->GetHit(Damage);
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