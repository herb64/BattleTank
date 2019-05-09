// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
	/// NOTE: this might inherit ticking from StaticMeshComponent base. But actually,
	///       do not add any additional code to switch off ticking, unless performance
	///       profiling makes this necessary.

	// Relative Speed: -1 to +1
	float Speed = MaxElevationSpeed * RelativeSpeed;

	// Barrel inherits from USceneComponent - very useful methods here
	FRotator BarrelRotator = GetForwardVector().Rotation();
	//FRotator AimRotator = AimDirection.Rotation();
	//FRotator DeltaRotator = AimRotator - BarrelRotator;

	UE_LOG(LogTemp, Warning, TEXT("%f > %s elevate with %f degrees per second"), GetWorld()->GetTimeSeconds(), *GetOwner()->GetName(), Speed);
}