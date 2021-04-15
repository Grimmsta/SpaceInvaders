#include "EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "../Projectile.h"
#include "../EnemyManager.h"

AEnemyCharacter::AEnemyCharacter()
{
	//Add mesh and collider
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(RootComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(BoxCollider);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.ObjectFlags = RF_Transient;
	SpawnParams.Owner = this;

	AActor* SpawningActor = GetOwner();
	EnemyManager = Cast<AEnemyManager>(SpawningActor);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Line Trace to find blocking walls
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + CurrentDirection * 10.f, ECC_WorldStatic);

	if (HitResult.bBlockingHit)
	{
		EnemyManager->CallChangeDirection();
	}


	//Move Enemy character
	MoveIntervalLeft -= DeltaTime;

	if (MoveIntervalLeft < 0)
	{
		MoveIntervalLeft = MoveInterval;

		FVector NewLocation = GetActorLocation() + CurrentDirection * MovementSpeed; //Move the enemy side to side

		SetActorLocation(NewLocation);
	}
}

void AEnemyCharacter::DestroyEnemy()
{
	EnemyManager->GetPlayerCharacter()->RecieveEnemyKilled(EnemyScoreValue);
	EnemyManager->RemoveEnemyFromActiveEnemies(this);

	this->Destroy(true);
}

void AEnemyCharacter::MoveDownAndChangeDirection()
{
	CurrentDirection = -CurrentDirection;
	SetActorLocation(GetActorLocation() - FVector::UpVector * MoveDownUnits);
}

void AEnemyCharacter::Shoot(AProjectile* ProjectileToShoot)
{
	ProjectileToShoot->MoveProjectile(GetActorLocation() - FVector::UpVector * ProjectileSpawnOffset, -FVector::UpVector, Target::PLAYER);
}

void AEnemyCharacter::HasReachedBottom()
{
	EnemyManager->GetPlayerCharacter()->GameOver();
}