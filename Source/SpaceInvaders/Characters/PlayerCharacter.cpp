#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "../Projectile.h"
#include "EnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(RootComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(BoxCollider);

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.ObjectFlags = RF_Transient;
	SpawnParams.Owner = this;

	MyPlayerController = GetWorld()->GetFirstPlayerController();
	MyPlayerController->bShowMouseCursor = false;

	SpawnProjectile();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Trace for blocking walls
	FHitResult HitResult;

	FVector CurrentMovingDirection = FVector::RightVector * CurrentDirection;
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + CurrentMovingDirection * 50.f, ECC_WorldStatic);

	if (HitResult.GetActor() != this && HitResult.bBlockingHit)
	{
		CurrentDirection = 0;
	}

	//Move player
	FVector NewLocation = GetActorLocation() + FVector::RightVector * CurrentDirection * MovementSpeed * DeltaTime;

	SetActorLocation(NewLocation);

	FireCooldownRemaining -= DeltaTime;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APlayerCharacter::HandleMovement);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::HandleFiring);
	PlayerInputComponent->BindAction(TEXT("QuitGame"), IE_Released, this, &APlayerCharacter::HandleExitGame);
}

void APlayerCharacter::HandleMovement(float Value)
{
	CurrentDirection = Value;
}

void APlayerCharacter::HandleFiring()
{
	if (FireCooldownRemaining > 0)
	{
		return;
	}

	AProjectile* Projectile = GetFreeProjectile();

	if (Projectile == nullptr)
	{
		SpawnProjectile();

		Projectile = GetFreeProjectile();
	}

	Projectile->MoveProjectile(GetActorLocation() + FVector::UpVector * ProjectileSpawnOffset, FVector::UpVector, Target::ENEMY);

	FireCooldownRemaining = FireCooldown;
}

void APlayerCharacter::HandleExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr, EQuitPreference::Quit, false);
}

void APlayerCharacter::RecieveEnemyKilled(float EnemyScoreValue)
{
	BP_UpdateScore(EnemyScoreValue);
}

void APlayerCharacter::RecieveGameWon()
{
	MyPlayerController->bShowMouseCursor = true;
	MyPlayerController->bEnableMouseOverEvents = true;
	MyPlayerController->bEnableClickEvents = true;
	BP_GameWon();
}

void APlayerCharacter::GameOver()
{
	BP_GameOver();
	MyPlayerController->bShowMouseCursor = true;
	MyPlayerController->bEnableMouseOverEvents = true;
	MyPlayerController->bEnableClickEvents = true;
	SetActorTickEnabled(false);
}

void APlayerCharacter::SpawnProjectile()
{
	AProjectile* NewProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	ProjectileInstances.Add(NewProjectile);
}

void APlayerCharacter::RemoveHealthPoint()
{
	HP--;

	BP_UpdateHealthPoints();

	if (HP <= 0)
	{
		GameOver();
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