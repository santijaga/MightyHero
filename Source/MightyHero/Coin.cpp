#include "Coin.h"
#include "MightyHeroGameModeBase.h"
#include "SoundController.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACoin::ACoin()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = RootComponent;
	ProjectileMovementComponent->InitialSpeed = 800.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void ACoin::BeginPlay()
{
	Super::BeginPlay();

	float ZDirection = FMath::RandRange(-0.5f, 0.5f);
	FVector LaunchDirection = FVector(1, 0, ZDirection);
	ProjectileMovementComponent->Velocity = LaunchDirection * ProjectileMovementComponent->InitialSpeed;
}

void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ControlCoinMovement(DeltaTime);
}

void ACoin::Collect()
{
	Super::Collect();

	if (ASoundController* SoundController = Cast<ASoundController>(Cast<AMightyHeroGameModeBase>(GetWorld()->GetAuthGameMode())->GetSoundController()))
	{
		SoundController->PlayShortCollectCue();
	}
}

void ACoin::ControlCoinMovement(float DeltaTime)
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerPawn)
	{
		TargetDirection = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	}

	if (!bIsAcceleratingTowardsPlayer)
	{
		if (ProjectileMovementComponent->Velocity.Size() > MinSpeedBeforeAccelerating)
		{
			SlowDownCoin(DeltaTime);
		}
		else
		{
			bIsAcceleratingTowardsPlayer = true;
			AccelerationSpeed = MinSpeedBeforeAccelerating;
		}
	}

	if (bIsAcceleratingTowardsPlayer)
	{
		SpeedUpCoin(DeltaTime);
	}
}

void ACoin::SlowDownCoin(float DeltaTime)
{
	FVector NewVelocity = ProjectileMovementComponent->Velocity - (ProjectileMovementComponent->Velocity.GetSafeNormal() * DecelerationRate * DeltaTime);
	ProjectileMovementComponent->Velocity = NewVelocity;
}

void ACoin::SpeedUpCoin(float DeltaTime)
{
	if (AccelerationSpeed < MaxSpeed)
	{
		AccelerationSpeed += AccelerationRate * DeltaTime;
		AccelerationSpeed = FMath::Min(AccelerationSpeed, MaxSpeed);
	}

	ProjectileMovementComponent->Velocity = TargetDirection * AccelerationSpeed;
}
