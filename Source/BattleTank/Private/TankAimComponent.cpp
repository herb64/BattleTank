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

void UTankAimComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	Turret = TurretToSet;
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

void UTankAimComponent::AimAt(FVector hitLocation, float LaunchSpeed)
{
	if (!Barrel)
	{
		UE_LOG(LogTemp, Error, TEXT("No Barrel Component - Check BP_Tank SetBarrelReference call"));
		return;
	}

	// Using component location here, later go for socket location when creating
	// a socket for the projectile at the top of the barrel
	//FVector barrelLocation = Barrel->GetComponentLocation();
	FVector startLocation = Barrel->GetSocketLocation(FName("Projectile"));

	if (Barrel->bDrawDebugLines) {
		DrawDebugLine(
			GetWorld(),
			startLocation,		// Projectile Start
			hitLocation,		// Hit Location
			FColor::Red,
			false,
			4.0f,
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
				4.0f,
				0,
				3.0f
			);
		}
		MoveBarrelTowards(AimDirection);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s: No aiming direction could be determined"), *GetOwner()->GetName());
	}
}

void UTankAimComponent::MoveBarrelTowards(FVector AimDirection)
{
	// Barrel inherits from USceneComponent - a lot of members from this are used here!!!
	// EXPERIMENT: GetComponentRotation() vs. GetForwardVector().Rotation()
	// Same results, BUT: Roll is not exactly 0.0 - see my documentation
	FRotator AimRotator = AimDirection.Rotation();
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator DeltaRotator = AimRotator - BarrelRotator;
	FRotator TurretRotator = Turret->GetForwardVector().Rotation();
	FRotator DeltaRotator2 = AimRotator - TurretRotator;
	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator2.Yaw);
}

