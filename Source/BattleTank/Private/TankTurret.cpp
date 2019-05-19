// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"


#include "Engine/StaticMesh.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"



// Just switch workaround off by commenting this
#define WORKAROUND

/*
 * W O R K A R O U N D   F O R   V A N I S H I N G   S T A T I C  M E S H
 * Set static mesh within the constructor manually
 */
UTankTurret::UTankTurret()
{
#ifdef WORKAROUND
	// We simply use the Engine Cone Basic Shape
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM(TEXT("/Game/Tank/Tank_fbx_Turret"));
	UStaticMesh* Mesh = SM.Object;
	if (Mesh != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Set Static Mesh %s"), *Mesh->GetName());
		this->SetStaticMesh(Mesh);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Could not set static mesh to component"));
	}
#endif
}

/*
 * Change design: instead of interpreting the YAW component parameter as relative speed and clamping
 * this to -1..+1 range, use it as what it really is: the required rotation angle for the turret to
 * hit the target point in terms of rotation around x axis.
 * The original approach:
 * Turret rotation uses the Yaw delta angle as speed. The higher the angle, the more way to go
 * and the higher the speed. Therefore clamp to -1..+1 to use as speed factor.
 * The problem with this approach: low frame rate and high rotation speed causes the rotation delta 
 * to increase, so that the rotation can exceed the target point. 
 * In the next frame, the same happens in the other direction.
 * The result is the turret jumping endlessly between 2 positions, like a dog wagging its tail.
 */
void UTankTurret::Rotate(float TargetHitYawAngle)
{
	// Using inherited USceneComponent members!!
	
	// ORIGINAL CODE
	float RelativeSpeed = FMath::Clamp(TargetHitYawAngle, -1.0f, 1.0f);
	float RotationDelta = MaxRotationSpeed * RelativeSpeed * GetWorld()->DeltaTimeSeconds;
	float NewRotation = RelativeRotation.Yaw + RotationDelta;
	SetRelativeRotation(FRotator(0.0f, NewRotation, 0.0f));

	// NEW CODE
	/*float MaxLimitedAngle = MaxRotationSpeed * FMath::Sign(TargetHitYawAngle) * GetWorld()->DeltaTimeSeconds;
	SetRelativeRotation(FRotator(0.0f,
		FMath::Abs(TargetHitYawAngle) > FMath::Abs(MaxLimitedAngle) ? 
		RelativeRotation.Yaw + MaxLimitedAngle : 
		RelativeRotation.Yaw + TargetHitYawAngle,
		0.0f));*/
}
