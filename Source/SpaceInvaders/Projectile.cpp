#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/EnemyCharacter.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = true;

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
	ProjectileTarget = InProjectileTarget;
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
		if (Player != GetOwner())
		{
			SetProjectileFree();

			Player->RemoveHealthPoint();
		}
	}
	else if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor))
	{
		if (Enemy != GetOwner())
		{
			SetProjectileFree();

			Enemy->DestroyEnemy();
		}
	}
}

bool AProjectile::IsFree()
{
	return bIsFree;
}

//OnCollision
	//Ignore owner	
	//if hit Player, remove one life
	//if hit enemy, destroy enemy
	//if hit bullet, MakeFree 