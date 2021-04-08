#pragma once

#include "GameFramework/Pawn.h"
#include "EnemyCharacter.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class AProjectile;

UCLASS()
class AEnemyCharacter : public APawn 
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;

	void DestroyEnemy();

	void Shoot();

	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = Collider)
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, Category = Shooting)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = Shooting)
	float ShootInterval = 0.7f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float MovementSpeed = 1000;

	float ShootIntervalLeft = 0;
	float DistanceMoved = 0;

	FActorSpawnParameters SpawnParams;

	FVector CurrentDirection = FVector::RightVector;
};