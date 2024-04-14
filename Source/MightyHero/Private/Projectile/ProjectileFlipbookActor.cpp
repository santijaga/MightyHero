
#include "Projectile/ProjectileFlipbookActor.h"

#include "Components/BoxComponent.h"
#include "Enemy/RPGEnemyFlipbookActor.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "Particles/ParticleFlipbookActor.h"

AProjectileFlipbookActor::AProjectileFlipbookActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create hitbox for projectile
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Box"));
	HitBox->SetupAttachment(RootComponent);
}

void AProjectileFlipbookActor::BeginPlay()
{
	Super::BeginPlay();

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectileFlipbookActor::OnHitTarget);

	GetWorldTimerManager().SetTimer(TimerHandle_FlyParticles, this, &AProjectileFlipbookActor::SpawnFlyParticles, 0.05f, true);
}

void AProjectileFlipbookActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
	TrackForFlyAway();
}

/*
* Event
*/
// On Component Begin Overlap Signature
void AProjectileFlipbookActor::OnHitTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("[OnHitTarget] %s"), *OtherActor->GetName());
	if (OtherActor->IsA(ARPGEnemyFlipbookActor::StaticClass()))
	{
		ARPGEnemyFlipbookActor* Enemy = Cast<ARPGEnemyFlipbookActor>(OtherActor);
		if (Enemy)
		{
			Enemy->GetHit(Damage);
			DestroySelf();
		}
	}
}

/*
* Behavior
*/
void AProjectileFlipbookActor::DestroySelf()
{
	Destroy();
}

void AProjectileFlipbookActor::Move(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	NewLocation += GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(NewLocation);
}

void AProjectileFlipbookActor::TrackForFlyAway()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Player)
	{
		float Distance = FVector::Dist(Player->GetActorLocation(), GetActorLocation());
		if (Distance > FlyAwayDistance)
		{
			DestroySelf();
		}
	}
}

void AProjectileFlipbookActor::SpawnFlyParticles()
{
	if (FlyParticles)
	{
		AParticleFlipbookActor* Particle = GetWorld()->SpawnActor<AParticleFlipbookActor>(FlyParticles, GetActorLocation(), FRotator::ZeroRotator);
	}
}