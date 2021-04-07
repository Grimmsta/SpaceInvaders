#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


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
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifeTime -= DeltaTime;

	DistanceMoved += ProjectileSpeed * DeltaTime;

	const FVector NewLocation = StartLocation + StartDirection * DistanceMoved;

	SetActorLocation(NewLocation);

	//TODO: destroy after 5 sec
}

void AProjectile::MoveProjectile(const FVector& InStartLocation, const FVector& InStartDirection)
{
	StartLocation = InStartLocation;
	StartDirection = InStartDirection;
	SetActorTickEnabled(true);
}