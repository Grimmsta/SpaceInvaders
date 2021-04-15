#pragma once 

#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

class AEnemyCharacter;
class AProjectile;
class APlayerCharacter;	

UCLASS()
class AEnemyManager : public AActor
{
	GENERATED_BODY()

public:	
	AEnemyManager();

	virtual void BeginPlay() override;

	void RemoveEnemyFromActiveEnemies(AEnemyCharacter* EnemyToRemove);

	void CallChangeDirection();

	APlayerCharacter* GetPlayerCharacter();

private:
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemies();

	void SpawnProjectile();

	AProjectile* GetFreeProjectile();

	UPROPERTY(EditAnywhere)
	APlayerCharacter* Player;

	UPROPERTY(EditAnywhere, Category = Spawning)
	int SpawnOffset = 10;	
	
	UPROPERTY(EditAnywhere, Category = Spawning, meta = (ClampMin = 1))
	int AmountOfEnemiesToSpawnOnXAxis = 11;
	
	UPROPERTY(EditAnywhere, Category = Spawning, meta = (ClampMin = 1))
	int AmountOfEnemiesToSpawnOnYAxis = 5;

	UPROPERTY(EditAnywhere, Category = Spawning)
	TSubclassOf<AEnemyCharacter> EnemyType1Class;

	UPROPERTY(EditAnywhere, Category = Spawning)
	TSubclassOf<AEnemyCharacter> EnemyType2Class;

	UPROPERTY(EditAnywhere, Category = Shooting)
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = Shooting)
	float MinShootingInterval = 1.f;

	UPROPERTY(EditAnywhere, Category = Shooting)
	float MaxShootingInterval = 5.f;
	
	float ShootInterval = 0;
	float ShootIntervalLeft = ShootInterval;

	bool bEnemiesChangingDirection = false;

	float MoveDownCooldown = 1.f;
	float MoveDownCooldownRemaining = MoveDownCooldown;

	TArray<AEnemyCharacter*> ActiveEnemies;
	TArray<AProjectile*> SpawnedProjectiles;

	FActorSpawnParameters SpawnParams;
};