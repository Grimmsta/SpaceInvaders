#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AProjectile;

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	void HandleMovement(float value);
	void HandleFiring();

	void SpawnProjectiles();

	AProjectile* GetFreeProjectile();

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = Shooting)
	float FireCooldown = .5f;

	TArray<AProjectile*> ProjectileInstances;

	FActorSpawnParameters SpawnParams;

	float PreCachedProjectilesAmount = 50;
	float FireCooldownLeft;
};
