#include "EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.h"
#include "../Projectile.h"
#include <Kismet/GameplayStatics.h>

AEnemyCharacter::AEnemyCharacter() 
{
	//Add mesh and collider
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
	SpawnParams.Instigator = this;
	SpawnParams.Owner = this;

	ShootIntervalLeft = ShootInterval;
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Todo: have a cooldown on the move, only move x units every y seconds

	ShootIntervalLeft -= DeltaTime;

	if (ShootIntervalLeft < 0)
	{
		Shoot();
		ShootIntervalLeft = ShootInterval;
		//Move these to another function, like when it collides with a wall
		CurrentDirection = -CurrentDirection; //Change direction
		SetActorLocation(GetActorLocation() - FVector::UpVector*10); //Move the enemy down
	}

	FVector NewLocation = GetActorLocation() + CurrentDirection * (MovementSpeed * DeltaTime); //Move the enemy side to side

	SetActorLocation(NewLocation);

	//Move target down
	//Check if we collided with wall/blocking volume
		//When we do, move down and change direction
		//Shoot random 
}

void AEnemyCharacter::DestroyEnemy()
{
	RootComponent->SetVisibility(false, true);
	SetActorTickEnabled(false);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (APlayerCharacter* Character = Cast<APlayerCharacter>(PlayerCharacter))
	{
		Character->RecieveEnemyKilled(EnemyScoreValue);
	}
}

void AEnemyCharacter::Shoot()
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, GetActorLocation(), GetActorRotation(), SpawnParams);

	Projectile->MoveProjectile(GetActorLocation() - FVector::UpVector * 100.f, -FVector::UpVector, Target::PLAYER);
}