#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerCharacter.generated.h"

class AProjectile;
class AEnemyCharacter;
class UBoxComponent;

UCLASS()
class APlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void RemoveHealthPoint();

	void RecieveEnemyKilled(float EnemyScoreValue);

	void RecieveGameWon();
	
	void GameOver();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_UpdateHealthPoints();	
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_UpdateScore(float ScoreToAdd);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_GameOver();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_GameWon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		int HP = 3;

protected:
	virtual void BeginPlay() override;

private:
	void HandleMovement(float value);
	void HandleFiring();
	void HandleExitGame();

	void SpawnProjectile();

	AProjectile* GetFreeProjectile();

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	UStaticMeshComponent* MeshComponent;
		
	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, Category = Shooting)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = Shooting)
	float FireCooldown = .5f;

	UPROPERTY(EditAnywhere, Category = Shooting)
	float ProjectileSpawnOffset = 50.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float MovementSpeed = 100;

	float CurrentDirection = 0;

	TArray<AProjectile*> ProjectileInstances;

	FActorSpawnParameters SpawnParams;

	APlayerController* MyPlayerController;

	float FireCooldownRemaining;
};