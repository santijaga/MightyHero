


#include "Pawn/RPGPawn.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "PaperFlipbookComponent.h"

void ARPGPawn::BeginPlay()
{
	Super::BeginPlay();

	if (UCharacterMovementComponent* MC = GetCharacterMovement())
	{
		MC->GravityScale = 0;
		MC->Velocity = FVector(0, 0, 0);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] [BeginPlay] UCharacterMovementComponent not found"), *this->GetName());
	}

	PawnState = ERPGCharacterStates::SE_Idle;
	PawnGameplayState = ERPGCharacterGameState::SE_StandBy;
}

void ARPGPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PawnGameplayState == ERPGCharacterGameState::SE_Playing)
	{
		Fly();
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
void ARPGPawn::StartGameplay()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Gameplay started"), *this->GetName());
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

		UE_LOG(LogTemp, Warning, TEXT("[%s] Current velocity = %s; Current gravity scale = %f"), *this->GetName(), *MC->Velocity.ToString(), MC->GravityScale);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Movement component not found"), *this->GetName());
	}
}

/*
* Animation
*/
void ARPGPawn::SelectFlipbook()
{
	if (UPaperFlipbookComponent* Flipbook = GetSprite())
	{
		if (PawnState == ERPGCharacterStates::SE_Fall)
		{
			if (Flipbook->GetFlipbook() != FlyFlipbook)
			{
				SetFlyFlipbook(Flipbook);
			}
		}

		if (PawnState == ERPGCharacterStates::SE_Rise)
		{
			if (Flipbook->GetFlipbook() != RiseFlipbook)
			{
				SetRiseFlipbook(Flipbook);
			}
		}
	}
}

void ARPGPawn::SetFlyFlipbook(UPaperFlipbookComponent* Flipbook)
{
	Flipbook->SetFlipbook(FlyFlipbook);
	Flipbook->SetLooping(true);
	Flipbook->PlayFromStart();
}

void ARPGPawn::SetRiseFlipbook(UPaperFlipbookComponent* Flipbook)
{
	Flipbook->SetFlipbook(RiseFlipbook);
	Flipbook->SetLooping(true);
	Flipbook->PlayFromStart();
}