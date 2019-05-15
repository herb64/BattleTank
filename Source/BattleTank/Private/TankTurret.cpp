// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
	// Turret also inherits from USceneComponent - a lot of members from this are used here!!!
	RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.0f, 1.0f);
	float RotationDelta = MaxRotationSpeed * RelativeSpeed * GetWorld()->DeltaTimeSeconds;
	float NewRotation = RelativeRotation.Yaw + RotationDelta;
	SetRelativeRotation(FRotator(0.0f, NewRotation, 0.0f));
}
