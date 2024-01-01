#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACharacterBase::StartGameplay()
{
	if (MovementComponent)
	{
		MovementComponent->GravityScale = 2.f;
	}

	ForwardSpeed = 500.f;
	JumpVelocity = 700.f;

	bIsMovementAllowed = true;
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

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetCharacterMovement();
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