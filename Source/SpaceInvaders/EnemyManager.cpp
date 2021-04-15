#include "EnemyManager.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"	

AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.ObjectFlags = RF_Transient;
	SpawnParams.Owner = this;

	ShootInterval = FMath::RandRange(MinShootingInterval, MaxShootingInterval);
	ShootIntervalLeft = ShootInterval;

	for (int i = 0; i < 10; i++)
	{
		SpawnProjectile();
	}

	SpawnEnemies();
}

void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ShootIntervalLeft -= DeltaTime;

	if (bEnemiesChangingDirection == true)
	{
		MoveDownCooldownRemaining -= DeltaTime;

		if (MoveDownCooldownRemaining <= 0)
		{
			bEnemiesChangingDirection = false;
			MoveDownCooldownRemaining = MoveDownCooldown;
		}
	}

	if (ShootIntervalLeft < 0 && ActiveEnemies.Num() > 0)
	{
		ShootInterval = FMath::RandRange(MinShootingInterval, MaxShootingInterval);

		ShootIntervalLeft = ShootInterval;

		int EnemyToShoot = FMath::RandRange(0, ActiveEnemies.Num() - 1);

		if (ActiveEnemies[EnemyToShoot] != nullptr)
		{
			AProjectile* ProjectileToShoot = GetFreeProjectile();

			if (ProjectileToShoot == nullptr)
			{
				SpawnProjectile();
				ProjectileToShoot = GetFreeProjectile();
			}

			ActiveEnemies[EnemyToShoot]->Shoot(ProjectileToShoot);
		}
	}
}

void AEnemyManager::SpawnEnemies()
{
	for (int row = 0; row < AmountOfEnemiesToSpawnOnXAxis; row++)
	{
		for (int column = 0; column < AmountOfEnemiesToSpawnOnYAxis; column++)
		{
			FVector SpawnOffsetVector = FVector::UpVector * SpawnOffset * column + FVector::RightVector * SpawnOffset * row;
			
			AEnemyCharacter* NewEnemyInstance;
			if (column < 2)
			{
				NewEnemyInstance = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyType1Class, GetActorLocation() +
					SpawnOffsetVector, GetActorRotation(), SpawnParams);
			}
			else if (column >= 2)
			{
				NewEnemyInstance = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyType2Class, GetActorLocation() +
					SpawnOffsetVector, GetActorRotation(), SpawnParams);
			}

			ActiveEnemies.Add(NewEnemyInstance);
		}
	}
}

void AEnemyManager::SpawnProjectile()
{
	AProjectile* NewProjectileInstance = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, GetActorLocation(),
		GetActorRotation(), SpawnParams);

	SpawnedProjectiles.Add(NewProjectileInstance);
}

AProjectile* AEnemyManager::GetFreeProjectile()
{
	for (AProjectile* Projectile : SpawnedProjectiles)
	{
		if (Projectile->IsFree())
		{
			return Projectile;
		}
	}
	return nullptr;
}

void AEnemyManager::RemoveEnemyFromActiveEnemies(AEnemyCharacter* EnemyToRemove)
{
	ActiveEnemies.Remove(EnemyToRemove);

	if (ActiveEnemies.Num() <= 0)
	{	
		Player->RecieveGameWon();
	}
}

void AEnemyManager::CallChangeDirection()
{
	if (bEnemiesChangingDirection == false)
	{
		bEnemiesChangingDirection = true;
		for (AEnemyCharacter* Enemy : ActiveEnemies)
		{
			if (Enemy != nullptr)
			{
				Enemy->MoveDownAndChangeDirection();
			}
		}
	}
}

APlayerCharacter* AEnemyManager::GetPlayerCharacter()
{
	if (Player != nullptr)
	{
		return Player;
	}
	return nullptr;
}