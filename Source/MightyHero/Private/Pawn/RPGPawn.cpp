


#include "Pawn/RPGPawn.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Core/MightyHeroRPGGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"

void ARPGPawn::BeginPlay()
{
	Super::BeginPlay();

	PawnState = ERPGCharacterStates::SE_Idle;
	PawnGameplayState = ERPGCharacterGameState::SE_StandBy;
	StartPoint = GetActorLocation().X;
}

void ARPGPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PawnGameplayState == ERPGCharacterGameState::SE_Playing)
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

	if (PawnGameplayState == ERPGCharacterGameState::SE_StandBy)
	{
		Stay();
	}

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
	if (PawnGameplayState == ERPGCharacterGameState::SE_Playing)
	{
		PawnState = ERPGCharacterStates::SE_Jump;
	}
}

/*
* Interface
*/
void ARPGPawn::GameOver()
{
	PawnGameplayState = ERPGCharacterGameState::SE_StandBy;
}

float ARPGPawn::GetDistance()
{
	return CalculateDistance();
}

void ARPGPawn::StartGame()
{
	PawnGameplayState = ERPGCharacterGameState::SE_Playing;
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
		if (PawnState == ERPGCharacterStates::SE_Jump)
		{
			NewVelocity.Z = BaseJumpVelocity;
		}
		else
		{
			NewVelocity.Z = CurrentVelocity.Z;
		}

		if (NewVelocity.Z > 0)
		{
			PawnState = ERPGCharacterStates::SE_Rise;
		}

		if (NewVelocity.Z < 0)
		{
			PawnState = ERPGCharacterStates::SE_Fall;
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
void ARPGPawn::SelectFlipbook()
{
	if (UPaperFlipbookComponent* Flipbook = GetSprite())
	{
		if (PawnState == ERPGCharacterStates::SE_Aim)
		{
			if (Flipbook->GetFlipbook() != AimFlipbook)
			{
				SetFlipbook(Flipbook, AimFlipbook, true);
			}
		}

		if (PawnState == ERPGCharacterStates::SE_Fall)
		{
			if (Flipbook->GetFlipbook() != FlyFlipbook)
			{
				SetFlipbook(Flipbook, FlyFlipbook, true);
			}
		}

		if (PawnState == ERPGCharacterStates::SE_Rise)
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