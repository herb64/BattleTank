// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
	/// NOTE: this might inherit ticking from StaticMeshComponent base. But actually,
	///       do not add any additional code to switch off ticking, unless performance
	///       profiling makes this necessary.

	// Barrel inherits from USceneComponent - a lot of members from this are used here!!!
	RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.0f, 1.0f);
	float ElevationDelta = MaxElevationSpeed * RelativeSpeed * GetWorld()->DeltaTimeSeconds;
	float RawNewElevation = RelativeRotation.Pitch + ElevationDelta;
	float ClampedElevation = FMath::Clamp(RawNewElevation, MinimumElevation, MaximumElevation);

	// TODO: this makes camera jitter
	SetRelativeRotation(FRotator(ClampedElevation, 0.0f, 0.0f));

	UE_LOG(LogTemp, Warning, TEXT("%f > %s elevation is now %f (Delta: %f)"), GetWorld()->GetTimeSeconds(), *GetOwner()->GetName(), ClampedElevation, ElevationDelta);
}