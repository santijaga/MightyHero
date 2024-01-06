#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MeteorActor.h"
#include "MightyHeroPlayerState.h"
#include "Components/CapsuleComponent.h"

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetCharacterMovement();

	OnActorBeginOverlap.AddDynamic(this, &ACharacterBase::OnOverlapBegin);
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
		MeteorActor->Destruction();
	}
}

void ACharacterBase::EarthDestroed()
{
	Stop();
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