#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"	
#include "Characters/PlayerCharacter.h"
#include "Characters/EnemyCharacter.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(RootComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile mesh"));
	MeshComponent->SetupAttachment(BoxCollider);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OverlapBegin);
	BoxCollider->SetCollisionProfileName(TEXT("NoCollision"));

	SetProjectileVisibility(false);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifeTimeLeft -= DeltaTime;

	if (LifeTimeLeft <= 0)
	{
		SetProjectileFree();
	}

	DistanceMoved += ProjectileSpeed * DeltaTime;

	const FVector NewLocation = StartLocation + StartDirection * DistanceMoved;

	SetActorLocation(NewLocation);
}

void AProjectile::MoveProjectile(const FVector& InStartLocation, const FVector& InStartDirection, Target InProjectileTarget)
{
	bIsFree = false;
	StartLocation = InStartLocation;
	StartDirection = InStartDirection;
	TargetToDestroy = InProjectileTarget;
	LifeTimeLeft = LifeTime;
	DistanceMoved = 0.f;
	SetActorLocation(StartLocation);
	SetActorTickEnabled(true);
	SetProjectileVisibility(true);
	BoxCollider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AProjectile::SetProjectileFree()
{
	bIsFree = true;
	SetActorTickEnabled(false);
	SetProjectileVisibility(false);
	BoxCollider->SetCollisionProfileName(TEXT("NoCollision"));
}

void AProjectile::SetProjectileVisibility(bool bVisible)
{
	RootComponent->SetVisibility(bVisible, true);
}

void AProjectile::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (TargetToDestroy == Target::PLAYER)
		{
			SetProjectileFree();
			Explode(GetActorLocation());
			Player->RemoveHealthPoint();
		}
	}
	else if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor))
	{
		if (TargetToDestroy == Target::ENEMY)
		{
			SetProjectileFree();
			Explode(Enemy->GetActorLocation());
			Enemy->DestroyEnemy();
		}
	}
	else
	{
		Explode(GetActorLocation());
		SetProjectileFree();
	}
}

void AProjectile::Explode(const FVector& ExplodeLocation)
{
	if (Explosion)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, ExplodeLocation, GetActorRotation(), true);
	}
}

bool AProjectile::IsFree()
{
	return bIsFree;
}