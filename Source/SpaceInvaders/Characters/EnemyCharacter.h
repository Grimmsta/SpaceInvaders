#pragma once

#include "GameFramework/Pawn.h"
#include "EnemyCharacter.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class AProjectile;
class APlayerCharacter;
class AEnemyManager;

UCLASS()
class AEnemyCharacter : public APawn 
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	void DestroyEnemy();
	void MoveDownAndChangeDirection();
	void Shoot(AProjectile* ProjectileToShoot);

	UFUNCTION(BlueprintCallable)
	void HasReachedBottom();

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = Collider)
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, Category = Shooting)
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = Shooting, 
	meta = (Tooltip = "Adds an offset from the enemy to spawn the projectile. The higher the float, the further away it will spawn from the enenmy"))
	float ProjectileSpawnOffset = 50.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	 float MovementSpeed = 1000.f;
	
	UPROPERTY(EditAnywhere, Category = Movement)
	 float MoveInterval = 0.5f;
	 
	 UPROPERTY(EditAnywhere, Category = Movement)
	 float MoveDownUnits = 20.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	 float MoveDownCooldown = 1.f;
	
	UPROPERTY(EditAnywhere, Category = Enemy)
	 int EnemyScoreValue = 100;

	FVector CurrentDirection = FVector::RightVector;

	float ShootIntervalLeft = 0.f;
	float MoveIntervalLeft = 0.f;

	AEnemyManager* EnemyManager = nullptr;

	FActorSpawnParameters SpawnParams;

};