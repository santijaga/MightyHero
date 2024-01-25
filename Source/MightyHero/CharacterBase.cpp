#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MeteorActor.h"
#include "SatelliteActor.h"
#include "MightyHeroPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "MightyHeroGameModeBase.h"
#include "SoundController.h"


void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetCharacterMovement();

	OnActorBeginOverlap.AddDynamic(this, &ACharacterBase::OnOverlapBegin);

	StatesEnum = ECharacterStates::SE_Idle;
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementComponent && bIsMovementAllowed)
	{
		FVector CurrentVelocity = MovementComponent->Velocity;
		FVector ForwardDirection = GetActorForwardVector();

		FVector NewVelocity = ForwardDirection * ForwardSpeed;
		if (bWasJump)
		{
			NewVelocity.Z = JumpVelocity;
			bWasJump = false;
		}
		else
		{
			NewVelocity.Z = CurrentVelocity.Z;
		}

		if (NewVelocity.Z > 0 && IsFreelyMoving())
		{
			StatesEnum = ECharacterStates::SE_Rise;
		}

		if (NewVelocity.Z < 0 && IsFreelyMoving())
		{
			StatesEnum = ECharacterStates::SE_Fall;
		}

		MovementComponent->Velocity = NewVelocity;
	}
}

void ACharacterBase::StartGameplay()
{
	if (MovementComponent)
	{
		MovementComponent->GravityScale = DefaultGravityScale;
	}

	ForwardSpeed = DefaultForwardSpeed;
	JumpVelocity = DefaultJumpVelocity;

	bIsMovementAllowed = true;
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACharacterBase::TouchPressed);
}

void ACharacterBase::CharacterJump()
{
	if (MovementComponent && bIsMovementAllowed)
	{
		bWasJump = true;
		if (ASoundController* SoundController = Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetSoundController())
		{
			SoundController->PlayJump();
		}
	}
}

void ACharacterBase::TouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	CharacterJump();
}

void ACharacterBase::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	AMeteorActor* MeteorActor = Cast<AMeteorActor>(OtherActor);
	if (MeteorActor)
	{
		GetPlayerState<AMightyHeroPlayerState>()->AddScore();
		StatesEnum = ECharacterStates::SE_Impact;

		if (!MeteorActor->IsPendingKill())
		{
			MeteorActor->Destruction();
		}
	}

	ASatelliteActor* SatelliteActor = Cast<ASatelliteActor>(OtherActor);
	if (SatelliteActor)
	{
		Stop();
		StatesEnum = ECharacterStates::SE_Sad;

		if (!SatelliteActor->IsPendingKill())
		{
			SatelliteActor->Destruction();
		}

		bCrashedInSatellite = true;
	}
}

void ACharacterBase::EarthDestroyed()
{
	Stop();
	StatesEnum = ECharacterStates::SE_Sad;
}

void ACharacterBase::Fall()
{
	Stop();
}

void ACharacterBase::Stop()
{
	bIsMovementAllowed = false;
	
	if (MovementComponent)
	{
		MovementComponent->GravityScale = 0;
		MovementComponent->Velocity = FVector(0, 0, 0);
		ForwardSpeed = 0;
		JumpVelocity = 0;
	}
}

void ACharacterBase::OnImpactEnded()
{
	StatesEnum = ECharacterStates::SE_Idle;
}

bool ACharacterBase::IsFreelyMoving()
{
	return StatesEnum == ECharacterStates::SE_Idle || StatesEnum == ECharacterStates::SE_Fall || StatesEnum == ECharacterStates::SE_Rise;
}

void ACharacterBase::ResetCharacter()
{
	StatesEnum = ECharacterStates::SE_Idle;
	bCrashedInSatellite = false;
	difficultyLevel = 1;
}

void ACharacterBase::IncreaseDifficulty()
{
	difficultyLevel++;

	if (JumpVelocityPerLevel.IsValidIndex(difficultyLevel))
	{
		JumpVelocity = JumpVelocityPerLevel[difficultyLevel];
	}

	if (GravityScalePerLevel.IsValidIndex(difficultyLevel))
	{
		MovementComponent->GravityScale = GravityScalePerLevel[difficultyLevel];
	}

}
