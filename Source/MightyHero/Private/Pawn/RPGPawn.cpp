


#include "Pawn/RPGPawn.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Core/MightyHeroRPGGameModeBase.h"
#include "Components/BoxComponent.h"
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
}

void ARPGPawn::TouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (PawnGameplayState == ERPGCharacterGameStates::SE_Playing)
	{
		bIsJumping = true;
	}
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
	bIsAttacking = true;
	bIsAiming = false;
	GetWorld()->GetTimerManager().ClearTimer(AimHandle);
}

void ARPGPawn::OnThreatColliderOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsAiming = true;
	GetWorld()->GetTimerManager().SetTimer(AimHandle, [this]() mutable
		{
			bIsAiming = false;
			GetWorld()->GetTimerManager().ClearTimer(AimHandle);
		}, 2.0f, false);
}