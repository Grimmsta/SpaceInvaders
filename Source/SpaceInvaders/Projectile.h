#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UBoxComponent;

UCLASS()
class AProjectile :public AActor {
	GENERATED_BODY()

public:
	AProjectile();
	
	virtual void Tick(float DeltaTime) override;
	
	void MoveProjectile(const FVector& InStartLocation, const FVector& InStartDirection);
	void Explode();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = Collision)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, Category = Collision)
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, Category = Projectile)
	float Speed = 10.f;
	
	UPROPERTY(EditAnywhere, Category = Projectile)
	float LifeTime = 5.f;

	FVector StartLocation = FVector::ZeroVector;
	FVector StartDirection = FVector::ZeroVector; 

	float DistanceMoved = 0.f;
	float ProjectileSpeed = 10.f;
};