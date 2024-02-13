#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MeteorActor.h"
#include "SatelliteActor.h"
#include "MightyHeroPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "MightyHeroGameModeBase.h"
#include "SoundController.h"
#include "Blower.h"
#include "MeteorController.h"
#include "CollectableBase.h"
#include "CollectablesController.h"
#include "AuraCollectable.h"
#include "Coin.h"

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
			SoundController->PlayJump(JumpCue);
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

		if (IsValid(MeteorActor))
		{
			FVector MeteorActorLocation = MeteorActor->GetActorLocation();

			MeteorActor->Destruction(true);
		}
	}

	ASatelliteActor* SatelliteActor = Cast<ASatelliteActor>(OtherActor);
	if (SatelliteActor)
	{
		Stop();
		StatesEnum = ECharacterStates::SE_Sad;

		if (IsValid(SatelliteActor))
		{
			SatelliteActor->Destruction();
		}

		bCrashedInSatellite = true;
	}


	if (ACollectableBase* Collectable = Cast<ACollectableBase>(OtherActor))
	{
		if (ACollectablesController* CollectablesController = Cast<ACollectablesController>(Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetCollectablesController()))
		{
			CollectablesController->CollectableBehaviour(Collectable);
		}
	}

	if (ACoin* Coin = Cast<ACoin>(OtherActor))
	{
		GetPlayerState<AMightyHeroPlayerState>()->AddCoin();
		Coin->Collect();
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

void ACharacterBase::IncreaseDifficulty(int32 NewDifficulty)
{
	difficultyLevel = NewDifficulty;

	UE_LOG(LogTemp, Warning, TEXT("%f Level"), difficultyLevel);

	if (JumpVelocityPerLevel.IsValidIndex(difficultyLevel))
	{
		JumpVelocity = JumpVelocityPerLevel[difficultyLevel];
	}

	if (GravityScalePerLevel.IsValidIndex(difficultyLevel))
	{
		MovementComponent->GravityScale = GravityScalePerLevel[difficultyLevel];
	}

	if (ForwardSpeedPerLevel.IsValidIndex(difficultyLevel))
	{
		ForwardSpeed = ForwardSpeedPerLevel[difficultyLevel];
	}
}
