// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

void UTankBarrel::Elevate(float TargetHitPitchAngle)
{
	/// NOTE: this might inherit ticking from StaticMeshComponent base. But actually,
	///       do not add any additional code to switch off ticking, unless performance
	///       profiling makes this necessary.

	// Barrel inherits from USceneComponent - a lot of members from this are used here!!!
	float RelativeSpeed = FMath::Clamp(TargetHitPitchAngle, -1.0f, 1.0f);
	float ElevationDelta = MaxElevationSpeed * RelativeSpeed * GetWorld()->DeltaTimeSeconds;
	float RawNewElevation = RelativeRotation.Pitch + ElevationDelta;
	float ClampedElevation = FMath::Clamp(RawNewElevation, MinimumElevation, MaximumElevation);
	SetRelativeRotation(FRotator(ClampedElevation, 0.0f, 0.0f));

	// NEW CODE
	/*float MaxLimitedAngle = MaxElevationSpeed * FMath::Sign(TargetHitPitchAngle) * GetWorld()->DeltaTimeSeconds;
	SetRelativeRotation(FRotator(0.0f,
		FMath::Abs(TargetHitYawAngle) > FMath::Abs(MaxLimitedAngle) ?
		RelativeRotation.Yaw + MaxLimitedAngle :
		RelativeRotation.Yaw + TargetHitYawAngle,
		0.0f));*/
}
