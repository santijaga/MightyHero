#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACharacterBase::StartGameplay()
{
	if (MovementComponent)
	{
		MovementComponent->GravityScale = 1.f;
	}

	ForwardSpeed = 500.f;
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementComponent)
	{
		FVector CurrentVelocity = MovementComponent->Velocity;
		FVector ForwardDirection = GetActorForwardVector();

		FVector NewVelocity = ForwardDirection * ForwardSpeed;
		NewVelocity.Z = CurrentVelocity.Z;

		MovementComponent->Velocity = NewVelocity;
	}
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetCharacterMovement();
}