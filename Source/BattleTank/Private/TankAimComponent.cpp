// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Public/DrawDebugHelpers.h"	// e.g. DrawDebugLine

// Sets default values for this component's properties
UTankAimComponent::UTankAimComponent()
{
	// TODO: should the aim component tick??
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

// Called when the game starts
void UTankAimComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UTankAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTankAimComponent::AimAt(FVector hitLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("TankAimComponent AimAt(%s, %f)"), *hitLocation.ToString(), LaunchSpeed);
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
		FVector AimDirection = OutSuggestedVelocity.GetSafeNormal();

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

		// OLD CODE - using the MoveBarrelTowards() function - defactored...
		//MoveBarrelTowards(AimDirection);

		// NEW CODE - Barrel Elevation
		FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
		//FRotator BarrelRotator = Barrel->GetComponentRotation();
		// GetComponentRotation() vs. GetForwardVector().Rotation() Almost same results, BUT: Roll is not exactly 0.0 - see my notes
		FRotator AimRotator = AimDirection.Rotation();
		Barrel->Elevate((AimRotator - BarrelRotator).Pitch);

		// NEW CODE - Turret Rotation - calculate Yaw ourselves instead of using Yaw from AimRotator
		//FVector BarrelLocation = Turret->GetSocketLocation(FName("Barrel"));
		FVector BarrelLocation = Barrel->GetComponentLocation();
		FRotator YawRotator = (hitLocation - BarrelLocation).Rotation();
		FRotator DeltaRotator = YawRotator - BarrelRotator;
		Turret->Rotate(FMath::Abs(DeltaRotator.Yaw) < 180 ? DeltaRotator.Yaw : -DeltaRotator.Yaw);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s: No aiming direction could be determined"), *GetOwner()->GetName());
	}
}


/// No more using this function...
void UTankAimComponent::MoveBarrelTowards(FVector AimDirection)
{
	FRotator AimRotator = AimDirection.Rotation();
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator DeltaRotator = AimRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);

	//FRotator TurretRotator = Turret->GetForwardVector().Rotation();
	//DeltaRotator = AimRotator - TurretRotator;
	
	if (FMath::Abs(DeltaRotator.Yaw) < 180) {
		Turret->Rotate(DeltaRotator.Yaw);
	} else 	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}

}

EFiringStatus UTankAimComponent::GetFiringStatus(void) {
	return FiringStatus;
}
