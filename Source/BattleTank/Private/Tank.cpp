// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Projectile.h"
#include "TankBarrel.h"
#include "TankTurret.h"


// Sets default values
ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;	// Tank does not need to tick
	UE_LOG(LogTemp, Warning, TEXT("[%s] HFCM: Constructor TANK in C++"), *GetName())
}


// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] HFCM: BeginPlay TANK in C++"), *GetName())
	Super::BeginPlay();		// NEEDED, otherwise Blueprint BeginPlay() not called!!!
}

// Called to bind functionality to input - strange, that intellisense often complains
// about AActor not having that member function, but ATank is a Pawn! 
// seems to be somewhat timing related... ???
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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
		float LaunchSpeed = 4000.0f;		// Fire not in focus - TODO for now just keek compiler quiet
		Projectile->Launch(LaunchSpeed);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s: No Projectile spawned"), *GetName());
	}

	lastFireTime = GetWorld()->GetTimeSeconds();
}