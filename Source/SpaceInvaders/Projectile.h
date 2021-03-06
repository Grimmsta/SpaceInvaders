#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UBoxComponent;
class UParticleEmitter;

UENUM()
enum class Target {
	PLAYER,
	ENEMY
};

UCLASS()
class AProjectile : public AActor 
{
	GENERATED_BODY()

public:
	AProjectile();

	virtual void Tick(float DeltaTime) override;

	void MoveProjectile(const FVector& InStartLocation, const FVector& InStartDirection, Target InProjectileTarget);
	void Explode(const FVector& ExplodeLocation);

	void SetProjectileFree();
	void SetProjectileVisibility(bool bVisible);

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsFree();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = Collision)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = Collision)
	UBoxComponent* BoxCollider;
		
	UPROPERTY(EditAnywhere, Category = Collision)
	UParticleSystem* Explosion;

	UPROPERTY(EditAnywhere, Category = Projectile)
	 float ProjectileSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = Projectile)
	 float LifeTime = 5.f;

	FVector StartDirection = FVector::ZeroVector;
	FVector StartLocation = FVector::ZeroVector;

	float LifeTimeLeft = 0.f;
	float DistanceMoved = 0.f;
	bool bIsFree = true; 

	Target TargetToDestroy;
};