// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "TankAimComponent.h"
#include "Projectile.h"
#include "TankBarrel.h"


// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;	// Tank does not need to tick
	TankAimComponent = CreateDefaultSubobject <UTankAimComponent>(FName("Aim Component"));
}


// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
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
	// TODO - not mentioned in course video at that point - might comment on that, but we need to initialize our local variable here
	Barrel = BarrelToSet;
	if (Barrel) {
		// Try to get some infos on Collision, which is blocked by "hidecategories"
		Barrel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Warning, TEXT("%s: Barrel collision enabled = %s "), *Barrel->GetOwner()->GetName(), (Barrel->GetCollisionEnabled() ? TEXT("True") : TEXT("False")))
	}
}

// This is a Blueprint Callable - Called in BP_Tank Event Graph BeginPlay()
void ATank::SetTurretReference(UTankTurret* TurretToSet)
{
	TankAimComponent->SetTurretReference(TurretToSet);
}


void ATank::AimAt(FVector hitLocation)
{
	// no need to protect pointers as added in construction ???
	TankAimComponent->AimAt(hitLocation, LaunchSpeed);
}

void ATank::Fire()
{
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
}