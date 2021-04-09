#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Projectile.h"
#include "Containers/Array.h"

APlayerCharacter::APlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnProjectiles();

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.ObjectFlags = RF_Transient;
	SpawnParams.Instigator = this;
	SpawnParams.Owner = this;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireCooldownLeft -= DeltaTime;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APlayerCharacter::HandleMovement);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::HandleFiring);
}

void APlayerCharacter::HandleMovement(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		AddMovementInput(FVector(0, 1, 0), Value);
	}
}

void APlayerCharacter::HandleFiring()
{
	if (FireCooldownLeft > 0)
	{
		return;
	}

	AProjectile* Projectile = GetFreeProjectile();

	if (Projectile == nullptr)
	{
		return;
	}

	Projectile->MoveProjectile(GetActorLocation() + FVector::UpVector * 100.f, FVector::UpVector);

	FireCooldownLeft = FireCooldown;
}

void APlayerCharacter::SpawnProjectiles()
{
	for (int i = 0; i < 20; i++)
	{
		AProjectile* NewProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		ProjectileInstances.Add(NewProjectile);
	}
}

void APlayerCharacter::RemoveHealthPoint()
{
	HP--;
	
	BP_UpdateHealthPoints();

	if (HP <=0)
	{
		//GameOver
	}
}

AProjectile* APlayerCharacter::GetFreeProjectile()
{
	for (AProjectile* Projectile : ProjectileInstances)
	{
		if (Projectile->IsFree())
		{
			return Projectile;
		}
	}

	return nullptr;
}