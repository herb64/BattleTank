// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Public/DrawDebugHelpers.h"	// e.g. DrawDebugLine

// Sets default values for this component's properties
UTankAimComponent::UTankAimComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

/// Blueprint based setting
void UTankAimComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
	if (!ensure(Barrel) || !ensure(Turret)) return;
	UE_LOG(LogTemp, Warning, TEXT("%s: AimComponent Initialize Barrel: %s, Turret: %s"), *GetOwner()->GetName(), *Barrel->GetName(), *Turret->GetName());
}


void UTankAimComponent::BeginPlay()
{
	// Calling Super:: makes it call the blueprint as well. Not having an overridden
	// Beginplay at all would do the same - so we can remove it for code shortness
	Super::BeginPlay();
	// Makes first fire() call after starting game not fire and set state to reloading
	lastFireTime = GetWorld()->GetTimeSeconds();
	FiringStatus = EFiringStatus::Reloading;
}


void UTankAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetWorld()->GetTimeSeconds() - lastFireTime < ReloadTime)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if (ensure(Barrel) && AimDirection.Equals(Barrel->GetForwardVector(), 0.01f)) {
		FiringStatus = EFiringStatus::Locked;
	}
	else {
		FiringStatus = EFiringStatus::Aiming;
	}
}

void UTankAimComponent::AimAt(FVector hitLocation)
{
	if (!ensure(Barrel) || !ensure(Turret)) return;

	FVector startLocation = Barrel->GetSocketLocation(FName("Projectile"));

	if (Barrel->bDrawDebugLines) {
		DrawDebugLine(
			GetWorld(),
			startLocation,		// Projectile Start
			hitLocation,		// Hit Location
			FColor::Red,
			false,
			2.0f,
			0,
			2.0f
		);
	}

	FVector OutSuggestedVelocity = FVector(0.0f);			// Aim direction when normalized

	// Launch Speed is very important. From Engine Source:
	// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/Engine/Private/GameplayStatics.cpp
	// "this will automatically fall back to line test if radius is small enough"
	// https://en.wikipedia.org/wiki/Projectile_motion - Angle required to hit coordinate (x,y)
	if (UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutSuggestedVelocity,								// returned velocity length = LaunchSpeed
		startLocation,
		hitLocation,
		LaunchSpeed,
		false,												// true: returns just a vector pointing UP
		0.0f,
		0.0f,
		ESuggestProjVelocityTraceOption::DoNotTrace			// NEEDED, ELSE GETTING MANY NON RESULTS
	)) {
		AimDirection = OutSuggestedVelocity.GetSafeNormal();

		if (Barrel->bDrawDebugLines) {
			DrawDebugLine(
				GetWorld(),
				startLocation,									// Projectile Socket
				startLocation + AimDirection * 300.0f,			// 3m into AimDirection
				FColor::Yellow,
				false,
				2.0f,
				0,
				3.0f
			);
		}

		// Barrel Elevation
		FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
		//FRotator BarrelRotator = Barrel->GetComponentRotation(); // Same, just Roll != 0.0
		FRotator AimRotator = AimDirection.Rotation();
		Barrel->Elevate((AimRotator - BarrelRotator).Pitch);

		// Turret Rotation - calculate Yaw ourselves instead of using Yaw from AimRotator
		FVector BarrelLocation = Barrel->GetComponentLocation();
		FRotator YawRotator = (hitLocation - BarrelLocation).Rotation();
		FRotator DeltaRotator = YawRotator - BarrelRotator;
		Turret->Rotate(FMath::Abs(DeltaRotator.Yaw) < 180 ? DeltaRotator.Yaw : -DeltaRotator.Yaw);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s: No aiming direction could be determined"), *GetOwner()->GetName());
	}
}


EFiringStatus UTankAimComponent::GetFiringStatus(void) {
	return FiringStatus;
}

void UTankAimComponent::Fire()
{
	// See also: FPlatformTime::Seconds() - returns double...
	// We only fire within minimal intervals set within blueprint Parameter ReloadTime
	if (FiringStatus == EFiringStatus::Reloading) return;
	
	if (!ensure(ProjectileBlueprint)) return;
	if (!ensure(Barrel)) return;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		Barrel->GetSocketLocation(FName("Projectile")),
		Barrel->GetSocketRotation(FName("Projectile"))		// needed for forward vector at launch
		);

	if (Projectile)
	{
		Projectile->Launch(LaunchSpeed);
		FiringStatus = EFiringStatus::Reloading;	// just to make it clean
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s: No Projectile spawned"), *GetName());
	}

	lastFireTime = GetWorld()->GetTimeSeconds();
}
