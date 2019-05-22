// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "TankAimComponent.h"
#include "Projectile.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "TankMovementComponent.h"


// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;	// Tank does not need to tick
	// no need to protect pointers as added in construction ???
	TankAimComponent = CreateDefaultSubobject <UTankAimComponent>(FName("Aim Component"));
	// no longer have "inherited" movement component - add this as custom component now
	//TankMovementComponent = CreateDefaultSubobject <UTankMovementComponent>(FName("Movement Component"));
}


// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	//TankMovementComponent = Cast<UTankMovementComponent>(GetDefaultSubobjectByName(FName("TankMovement")));
	//if (!ensure(TankMovementComponent)) return;
	//TankMovementComponent->InitializeViaCpp();
}


// Called to bind functionality to input - strange, that intellisense often complains
// about AActor not having that member function, but ATank is a Pawn! 
// seems to be somewhat timing related... ???
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// This is a Blueprint Callable - Called in BP_Tank Event Graph BeginPlay()
void ATank::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	TankAimComponent->SetBarrelReference(BarrelToSet);
	Barrel = BarrelToSet;
	if (Barrel) {
		//Barrel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Warning, TEXT("%s: Barrel collision %s "), *Barrel->GetOwner()->GetName(), (Barrel->GetCollisionEnabled() ? TEXT("ENABLED") : TEXT("DISABLED")));
	}
}

// This is a Blueprint Callable - Called in BP_Tank Event Graph BeginPlay()
void ATank::SetTurretReference(UTankTurret* TurretToSet)
{
	TankAimComponent->SetTurretReference(TurretToSet);
	if (TurretToSet) {
		UE_LOG(LogTemp, Warning, TEXT("%s: Turret collision %s "), *TurretToSet->GetOwner()->GetName(), (TurretToSet->GetCollisionEnabled() ? TEXT("ENABLED") : TEXT("DISABLED")));
	}
}


void ATank::AimAt(FVector hitLocation)
{
	TankAimComponent->AimAt(hitLocation, LaunchSpeed);
}

void ATank::Fire()
{
	// See also: FPlatformTime::Seconds() - returns double...
	// We only fire within minimal intervals set within blueprint Parameter ReloadTime
	if (GetWorld()->GetTimeSeconds() - lastFireTime < ReloadTime) return;


	UE_LOG(LogTemp, Error, TEXT("FIRE"));
	if (!Barrel)
	{
		UE_LOG(LogTemp, Error, TEXT("No Barrel at FIRE"));
		return;
	}

	/// See http://api.unrealengine.com/INT/API/Runtime/Engine/Engine/UWorld/SpawnActor/4/?lang=ja

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		Barrel->GetSocketLocation(FName("Projectile")),
		//FRotator::ZeroRotator
		Barrel->GetSocketRotation(FName("Projectile"))		// needed for forward vector at launch
		);

	if (Projectile)
	{
		Projectile->Launch(LaunchSpeed);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s: No Projectile spawned"), *GetName());
	}

	lastFireTime = GetWorld()->GetTimeSeconds();
}