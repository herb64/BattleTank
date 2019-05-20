// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

void UTankBarrel::Elevate(float TargetHitPitchAngle)	// TODO declaration var name not consistent
{
	/// NOTE: this might inherit ticking from StaticMeshComponent base. But actually,
	///       do not add any additional code to switch off ticking, unless performance
	///       profiling makes this necessary.

	// Using inherited USceneComponent members!!

	// OLD CODE
	/*float RelativeSpeed = FMath::Clamp(TargetHitPitchAngle, -1.0f, 1.0f);
	float ElevationDelta = MaxElevationSpeed * RelativeSpeed * dtime;
	float RawNewElevation = RelativeRotation.Pitch + ElevationDelta;
	float ClampedElevation = FMath::Clamp(RawNewElevation, MinimumElevation, MaximumElevation);
	SetRelativeRotation(FRotator(ClampedElevation, 0.0f, 0.0f));*/

	// NEW CODE
	float MaxLimitedAngle = MaxElevationSpeed * FMath::Sign(TargetHitPitchAngle) * GetWorld()->DeltaTimeSeconds;
	float RawNewElevation = FMath::Abs(TargetHitPitchAngle) > FMath::Abs(MaxLimitedAngle) ?
		RelativeRotation.Pitch + MaxLimitedAngle :
		RelativeRotation.Pitch + TargetHitPitchAngle;
	SetRelativeRotation(FRotator(FMath::Clamp(RawNewElevation, MinimumElevation, MaximumElevation)));
}
